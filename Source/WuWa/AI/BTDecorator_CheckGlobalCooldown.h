// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckGlobalCooldown.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTDecorator_CheckGlobalCooldown : public UBTDecorator
{
	GENERATED_BODY()

public:
    UBTDecorator_CheckGlobalCooldown();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
    UPROPERTY(EditAnywhere, Category = "Condition")
    struct FBlackboardKeySelector NextAttackTimeKey;
};
