#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CombatTeamInterface.h"
#include "LunoArrow.generated.h"

UCLASS()
class WUWA_API ALunoArrow : public AActor, public ICombatTeamInterface
{
	GENERATED_BODY()
	
public:
	ALunoArrow();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	void SpawnHitEffect(bool bHasTargetDamaged);
	bool CheckAttackHit();

public:
	FORCEINLINE virtual ETeamType GetTeamType() const override { return TeamType; }

private:
	UPROPERTY()
	ETeamType TeamType = ETeamType::Neutral;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> DamagedActors;

private:
	UPROPERTY(VisibleAnywhere, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

private:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> ArrowEffect;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> HitEffect_Cycle;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> HitEffect_Shock;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> HitEffect_Block;

private:
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> AttackHitEffect;
};
