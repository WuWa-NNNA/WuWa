// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/Player/PlayerStatComponent.h"
#include "Character/Resonator/Luno/Luno.h"
#include "LunoStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBaseAttackDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeCrescentTimeDelegate, float /*currentDash*/);
DECLARE_MULTICAST_DELEGATE(FOnBaseEndCrescentTimeDelegate);

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLunoIconGroup
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<TObjectPtr<class UPaperSprite>> Icons;
};

UCLASS()
class WUWA_API ULunoStatComponent : public UPlayerStatComponent
{
	GENERATED_BODY()
	
public :
	ULunoStatComponent();

	FOnBaseAttackDelegate OnBaseAttack;
	FOnChangeCrescentTimeDelegate OnChangeCrescentTime;
	FOnBaseEndCrescentTimeDelegate OnBaseEndCrescentTime;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void AttackChange();

	UFUNCTION()
	void ChangeAttackMode(ELunoState currentStatMode);

	void SetCrescentTimer();

private:
	void RevertLunoState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TMap<ELunoState, FLunoIconGroup> AttackIcon_Luno;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LunoStat", meta = (AllowPrivateAccess = "true"))
	float CrescentTime = 0.0f;
	FTimerHandle CrescentTimer;
};
