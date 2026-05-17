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
	AWWCharacter();

public:
	FORCEINLINE virtual ETeamType GetTeamType() const override { return TeamType; }

public:
	virtual void CheckAttackHit(const FAttackHitData& AttackHitData, TSet<TObjectPtr<AActor>>& DamagedActors) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	ETeamType TeamType = ETeamType::Neutral;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UWWStatComponent> Stat;
};
