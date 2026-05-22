#include "Character/WWCharacter.h"
#include "Stat/WWStatComponent.h"
#include "Physics/WWCollision.h"

#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Camera/CameraShakeBase.h"

AWWCharacter::AWWCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Stat = CreateDefaultSubobject<UWWStatComponent>(TEXT("Stat"));
}

void AWWCharacter::CheckAttackHit(const FAttackHitData& AttackHitData, TSet<TObjectPtr<AActor>>& DamagedActors)
{
	TArray<FHitResult> OutHitResults;

	const float AttackRange = AttackHitData.Range;
	const float AttackRadius = AttackHitData.Radius;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FVector OriginalStart = GetActorTransform().TransformPosition(AttackHitData.StartOffset);
	FVector OriginalEnd = OriginalStart + AttackHitData.Direction * AttackRange;

	FVector CapsuleOrigin = OriginalStart + (OriginalEnd - OriginalStart) * 0.5f;

	float SweepHalfLength = FMath::Max(0.0f, (AttackRange * 0.5f) - AttackRadius);

	FVector SweepStart = CapsuleOrigin - (GetActorForwardVector() * SweepHalfLength);
	FVector SweepEnd = CapsuleOrigin + (GetActorForwardVector() * SweepHalfLength);

	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, SweepStart, SweepEnd, FQuat::Identity, CCHANNEL_WWACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	UE_LOG(LogTemp, Warning, TEXT("Direction: %s, Range: %f"), *AttackHitData.Direction.ToString(), AttackRange);
#if ENABLE_DRAW_DEBUG
	const float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHitDetected ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
#endif

	if (!bHitDetected)
	{
		return;
	}

	ICombatTeamInterface* MyTeamActor = Cast<ICombatTeamInterface>(this);
	if (!MyTeamActor)
	{
		return;
	}

	bool bDidShakeCamera = false;

	for (const FHitResult& HitResult : OutHitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
		{
			continue;
		}

		if (DamagedActors.Contains(HitActor))
		{
			continue;
		}

		ICombatTeamInterface* HitTeamActor = Cast<ICombatTeamInterface>(HitActor);
		if (!HitTeamActor)
		{
			continue;
		}

		if (HitTeamActor->GetTeamType() == MyTeamActor->GetTeamType())
		{
			continue;
		}

		OnAttackSucceeded(DamagedActors, HitActor, HitResult, bDidShakeCamera);
	}
}

void AWWCharacter::OnAttackSucceeded(TSet<TObjectPtr<AActor>>& DamagedActors, AActor* HitActor, const FHitResult& HitResult, bool& bDidShakeCamera)
{
	DamagedActors.Add(HitActor);

	const float AttackDamage = 1.0f;

	FDamageEvent DamageEvent;
	float ActualDamage = HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	if (ActualDamage > 0.0f)
	{
		PlayDamagedSkin(HitActor, AttackDamage);
		SpawnAttackHitEffect(HitResult);
		PlayCameraShake(bDidShakeCamera);
	}
}

float AWWCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.0f && Stat)
	{
		Stat->ApplyDamage(ActualDamage);
	}

	return ActualDamage;
}

void AWWCharacter::SpawnAttackHitEffect(const FHitResult& HitResult)
{
	if (AttackHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	}
}

void AWWCharacter::PlayCameraShake(bool& bDidShakeCamera)
{
	if (!bDidShakeCamera)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && CameraShakeClass)
		{
			bDidShakeCamera = true;
			PC->ClientStartCameraShake(CameraShakeClass);
		}
	}
}

void AWWCharacter::PlayDamagedSkin(AActor* HitActor, float damage)
{
	int32 RandomInt = FMath::RandRange(1, 10);

	if (!Stat->DamageTextActorClass)
	{
		UE_LOG(LogTemp, Log, TEXT("no DamageTextActorClass"));
		return;
	}

	UWorld* world = GetWorld();
	if (world)
	{
		AActor* DamageActor = world->SpawnActorDeferred<AActor>(
			Stat->DamageTextActorClass,
			HitActor->GetTransform(),
			HitActor,
			HitActor->GetInstigator(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		if (DamageActor)
		{
			FProperty* Property = DamageActor->GetClass()->FindPropertyByName(TEXT("Dmg"));

			if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
			{
				DoubleProperty->SetPropertyValue_InContainer(DamageActor, static_cast<double>(damage + RandomInt));
			}
			else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
			{
				FloatProperty->SetPropertyValue_InContainer(DamageActor, damage + RandomInt);
			}
			DamageActor->FinishSpawning(HitActor->GetTransform());
		}
	}
}
