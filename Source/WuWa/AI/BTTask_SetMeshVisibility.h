// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMeshVisibility.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTTask_SetMeshVisibility : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_SetMeshVisibility();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Visibility")
    bool bHideMesh;
};
