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

	if (!BossCharacter || !TargetActor) return EBTNodeResult::Failed;

	FVector BossLoc = BossCharacter->GetActorLocation();
	FVector TargetLoc = TargetActor->GetActorLocation();

	float CurrentDistance = FVector::Dist2D(BossLoc, TargetLoc);
	float AppliedDistance = DistanceFromPlayer;

	if (!bForceExactDistance && CurrentDistance <= DistanceFromPlayer)
	{
		if (FMath::IsNearlyZero(AddYawOffset))
		{
			BlackboardComp->SetValueAsVector(IdealStartPosKey.SelectedKeyName, BossLoc);
			return EBTNodeResult::Succeeded;
		}
		AppliedDistance = CurrentDistance;
	}

	FVector DirFromTargetToBoss = (BossLoc - TargetLoc).GetSafeNormal2D();

	DirFromTargetToBoss = DirFromTargetToBoss.RotateAngleAxis(AddYawOffset, FVector::UpVector);

	FVector IdealStartLocation = TargetLoc + (DirFromTargetToBoss * AppliedDistance);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		FNavLocation ProjectedLocation;
		if (NavSys->ProjectPointToNavigation(IdealStartLocation, ProjectedLocation, FVector(1000.f, 1000.f, 1000.f)))
		{
			BlackboardComp->SetValueAsVector(IdealStartPosKey.SelectedKeyName, ProjectedLocation.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}