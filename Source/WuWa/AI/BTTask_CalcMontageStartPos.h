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
    // 에디터에서 수동으로 입력할 플레이어와의 간격 (언리얼 유닛 단위, 예: 300 = 3m)
    UPROPERTY(EditAnywhere, Category = "Positioning")
    float DistanceFromPlayer = 300.0f;

    // 타겟(플레이어) 액터를 가져올 블랙보드 키
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetKey;

    // 계산된 최적의 시작 좌표(Vector)를 저장할 블랙보드 키
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector IdealStartPosKey;
};