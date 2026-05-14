// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetGlobalCooldown.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTTask_SetGlobalCooldown : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_SetGlobalCooldown();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = "Cooldown")
    float CooldownTime = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector NextAttackTimeKey;
};
