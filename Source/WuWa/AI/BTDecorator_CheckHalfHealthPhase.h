// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckHalfHealthPhase.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTDecorator_CheckHalfHealthPhase : public UBTDecorator
{
	GENERATED_BODY()

public:
    UBTDecorator_CheckHalfHealthPhase();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
    UPROPERTY(EditAnywhere, Category = "Condition")
    float HealthThreshold = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Condition")
    struct FBlackboardKeySelector HealthRatioKey;

    UPROPERTY(EditAnywhere, Category = "Condition")
    struct FBlackboardKeySelector IsPhase2Key;
};
