#include "BTTask_AddYawRotation.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AddYawRotation::UBTTask_AddYawRotation()
{
	NodeName = TEXT("Turn To Target And Add Yaw");
}

EBTNodeResult::Type UBTTask_AddYawRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp) return EBTNodeResult::Failed;

	ACharacter* BossCharacter = Cast<ACharacter>(AIController->GetPawn());
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!BossCharacter || !TargetActor) return EBTNodeResult::Failed;

	FVector DirectionToTarget = TargetActor->GetActorLocation() - BossCharacter->GetActorLocation();

	//DirectionToTarget.Z = 0.0f;

	FRotator TargetRotation = DirectionToTarget.Rotation();

	TargetRotation.Yaw += AddYawOffset;

	BossCharacter->SetActorRotation(TargetRotation);

	return EBTNodeResult::Succeeded;
}