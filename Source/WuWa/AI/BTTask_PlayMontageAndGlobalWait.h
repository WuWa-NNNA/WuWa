// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayMontageAndGlobalWait.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTTask_PlayMontageAndGlobalWait : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_PlayMontageAndGlobalWait();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
    UPROPERTY(EditAnywhere, Category = "Montage")
    UAnimMontage* MontageToPlay;
};
