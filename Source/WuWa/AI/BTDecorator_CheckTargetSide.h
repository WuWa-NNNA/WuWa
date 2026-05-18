// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckTargetSide.generated.h"

UENUM(BlueprintType)
enum class ETargetSide : uint8
{
	Left    UMETA(DisplayName = "Left"),
	Right   UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class WUWA_API UBTDecorator_CheckTargetSide : public UBTDecorator
{
	GENERATED_BODY()

public:
    UBTDecorator_CheckTargetSide();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
    UPROPERTY(EditAnywhere, Category = "Condition")
    ETargetSide SideToCheck = ETargetSide::Left;

    UPROPERTY(EditAnywhere, Category = "Condition")
    struct FBlackboardKeySelector TargetKey;
};
