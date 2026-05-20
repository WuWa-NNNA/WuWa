#include "Projectile/LunoArrow.h"
#include "Physics/WWCollision.h"

#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ALunoArrow::ALunoArrow()
{
	TeamType = ETeamType::Resonator;

	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("Projectile"));
	SetRootComponent(Collision);
	Collision->OnComponentHit.AddDynamic(this, &ALunoArrow::OnHit);

	ArrowEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ArrowEffect"));
	ArrowEffect->SetupAttachment(RootComponent);
	ArrowEffect->SetVariableBool(TEXT("User.AudioOn"), false);
	ArrowEffect->SetVariableBool(TEXT("User.NoLaunchVFX"), false);
	ArrowEffect->SetVariableFloat(TEXT("User.ColorHue"), 0.85f);
	ArrowEffect->SetVariableFloat(TEXT("User._Size"), 1.0f);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 6000.0f;
	ProjectileMovement->MaxSpeed = 6000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ALunoArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsValid(OtherActor) || OtherActor == GetOwner())
	{
		return;
	}

	SpawnHitEffect(CheckAttackHit());

	Destroy();
}

void ALunoArrow::SpawnHitEffect(bool bHasTargetDamaged)
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z -= (bHasTargetDamaged ? 150.0f : 0.0f);

	if (HitEffect_Cycle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect_Cycle, SpawnLocation, FRotator::ZeroRotator, FVector(1.3f, 1.3f, 1.15f));
	}

	if (HitEffect_Shock)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect_Shock, SpawnLocation, FRotator::ZeroRotator, FVector(1.3f, 1.3f, 1.3f));
	}

	if (HitEffect_Block)
	{
		UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect_Block, SpawnLocation);
		SpawnedEffect->SetVariableFloat(TEXT("User._Size"), 3.0f);
		SpawnedEffect->SetVariableBool(TEXT("User.AudioOn"), false);
	}
}

bool ALunoArrow::CheckAttackHit()
{
	TArray<FHitResult> OutHitResults;

	const float AttackRange = 0.0f;
	const float AttackRadius = 300.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FVector Start = GetActorLocation();
	FVector End = Start;

	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_WWACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

//#if ENABLE_DRAW_DEBUG
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	const float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = bHitDetected ? FColor::Green : FColor::Red;
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
//#endif

	if (!bHitDetected)
	{
		return false;
	}

	ICombatTeamInterface* MyTeamActor = Cast<ICombatTeamInterface>(this);
	if (!MyTeamActor)
	{
		return false;
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

		DamagedActors.Add(HitActor);

		if (AttackHitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
		}

		if (!bDidShakeCamera)
		{
			APlayerController* PC = Cast<APlayerController>(GetInstigatorController());
			if (PC && CameraShakeClass)
			{
				bDidShakeCamera = true;
				PC->ClientStartCameraShake(CameraShakeClass);
			}
		}

		const float AttackDamage = 100.0f;

		FDamageEvent DamageEvent;
		HitActor->TakeDamage(AttackDamage, DamageEvent, GetInstigatorController(), this);
	}

	return true;
}