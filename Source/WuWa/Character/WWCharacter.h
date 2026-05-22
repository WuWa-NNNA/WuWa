#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Stat/WWStatComponent.h"
#include "Interface/CombatTeamInterface.h"
#include "Interface/AttackAnimNotifyInterface.h"
#include "WWCharacter.generated.h"

UCLASS()
class WUWA_API AWWCharacter : public ACharacter, public ICombatTeamInterface, public IAttackAnimNotifyInterface
{
	GENERATED_BODY()

public:
	AWWCharacter(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE virtual ETeamType GetTeamType() const override { return TeamType; }

	FORCEINLINE class UWWStatComponent* GetStatComponent() const { return Stat; }

public:
	virtual void CheckAttackHit(const FAttackHitData& AttackHitData, TSet<TObjectPtr<AActor>>& DamagedActors) override;
	virtual void OnAttackSucceeded(TSet<TObjectPtr<AActor>>& DamagedActors, AActor* HitActor, const FHitResult& HitResult, bool& bDidShakeCamera);

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SpawnAttackHitEffect(const FHitResult& HitResult);
	virtual void PlayCameraShake(bool& bDidShakeCamera);

private:
	void PlayDamagedSkin(AActor* HitActor, float damage);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	ETeamType TeamType = ETeamType::Neutral;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> AttackHitEffect;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UWWStatComponent> Stat;
};
