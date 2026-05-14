// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EDistanceRange.h"

#include "BTService_CalculateDistance.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UBTService_CalculateDistance : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CalculateDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector DistanceRangeKey;
};
