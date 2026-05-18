// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/PlayerStatComponent.h"
#include "Character/Resonator/Luno/Luno.h"
#include "LunoStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBaseAttackDelegate);

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillRStartDelegate, float);

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

	UFUNCTION()
	void AttackChange();

	UFUNCTION()
	void ChangeAttackMode(ELunoState currentStatMode);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TMap<ELunoState, FLunoIconGroup> AttackIcon_Luno;
};
