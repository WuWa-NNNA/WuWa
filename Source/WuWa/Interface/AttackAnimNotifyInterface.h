#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackAnimNotifyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAttackAnimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FAttackHitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem> HitEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitEffectScale = FVector(1.0f, 1.0f, 1.0f);
};

class WUWA_API IAttackAnimNotifyInterface
{
	GENERATED_BODY()

public:
	virtual void CheckAttackHit(const FAttackHitData& AttackHitData, TSet<TObjectPtr<AActor>>& DamagedActors) = 0;
};
