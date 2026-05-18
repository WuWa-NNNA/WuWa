// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Phase2Transition.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTTask_Phase2Transition : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_Phase2Transition();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
    UPROPERTY(EditAnywhere, Category = "Montage")
    UAnimMontage* TransitionMontage;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector IsPhase2Key;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector HealthRatioKey;
};
