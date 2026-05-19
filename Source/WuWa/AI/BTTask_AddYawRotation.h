// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AddYawRotation.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTTask_AddYawRotation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_AddYawRotation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AddYawOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetKey;
};
