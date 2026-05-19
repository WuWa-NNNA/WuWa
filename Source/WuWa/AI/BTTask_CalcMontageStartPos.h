#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CalcMontageStartPos.generated.h"

UCLASS()
class UBTTask_CalcMontageStartPos : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_CalcMontageStartPos();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = "Positioning")
    float DistanceFromPlayer = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Positioning")
    bool bForceExactDistance = false;

    UPROPERTY(EditAnywhere, Category = "Positioning")
    float AddYawOffset = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector IdealStartPosKey;
};