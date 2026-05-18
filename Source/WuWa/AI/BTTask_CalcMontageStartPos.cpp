#include "BTTask_CalcMontageStartPos.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_CalcMontageStartPos::UBTTask_CalcMontageStartPos()
{
    NodeName = TEXT("Calc Montage Start Pos");
}

EBTNodeResult::Type UBTTask_CalcMontageStartPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!AIController || !BlackboardComp) return EBTNodeResult::Failed;

    ACharacter* BossCharacter = Cast<ACharacter>(AIController->GetPawn());
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

    // 몽타주 검사가 필요 없어졌으므로 필수 액터 유효성만 검사합니다.
    if (!BossCharacter || !TargetActor) return EBTNodeResult::Failed;

    FVector BossLoc = BossCharacter->GetActorLocation();
    FVector TargetLoc = TargetActor->GetActorLocation();

    // 1. 보스의 현재 위치에서 타겟(플레이어)을 향하는 2D 직선 방향 계산
    FVector DirectionToTarget = (TargetLoc - BossLoc).GetSafeNormal2D();

    // 2. 타겟 위치에서 해당 방향의 반대쪽으로 '수동 입력한 거리(DistanceFromPlayer)'만큼 떨어진 좌표 도출
    FVector IdealStartLocation = TargetLoc - (DirectionToTarget * DistanceFromPlayer);

    // 3. 계산된 허공의 좌표를 내비메시 위의 유효한 바닥 좌표로 투영
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSys)
    {
        FNavLocation ProjectedLocation;
        if (NavSys->ProjectPointToNavigation(IdealStartLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
        {
            BlackboardComp->SetValueAsVector(IdealStartPosKey.SelectedKeyName, ProjectedLocation.Location);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}