#include "Character/WWCharacter.h"
#include "Physics/WWCollision.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

AWWCharacter::AWWCharacter()
{
}

void AWWCharacter::CheckAttackHit(const FAttackHitData& AttackHitData, TSet<TObjectPtr<AActor>>& DamagedActors)
{
	TArray<FHitResult> OutHitResults;

	const float AttackRange = AttackHitData.Range;
	const float AttackRadius = AttackHitData.Radius;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FVector Start = GetActorLocation()
		+ GetActorForwardVector() * AttackHitData.StartOffset.X
		+ GetActorRightVector() * AttackHitData.StartOffset.Y
		+ GetActorUpVector() * AttackHitData.StartOffset.Z;
	FVector End = Start + AttackHitData.Direction * AttackRange;

	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_WWACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

//#if ENABLE_DRAW_DEBUG
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	const float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = bHitDetected ? FColor::Green : FColor::Red;
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
//#endif

	if (!bHitDetected)
	{
		return;
	}

	ICombatTeamInterface* MyTeamActor = Cast<ICombatTeamInterface>(this);
	if (!MyTeamActor)
	{
		return;
	}

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

		if (AttackHitData.HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitData.HitEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), AttackHitData.HitEffectScale);
		}

		const float AttackDamage = 0.0f;

		FDamageEvent DamageEvent;
		HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
}
