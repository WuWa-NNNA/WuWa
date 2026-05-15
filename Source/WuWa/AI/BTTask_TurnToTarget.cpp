// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Instant Turn To Target");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BlackboardComp || !AIController) return EBTNodeResult::Failed;

	ACharacter* BossPawn = Cast<ACharacter>(AIController->GetPawn());
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (BossPawn && TargetActor)
	{
		FVector Direction = TargetActor->GetActorLocation() - BossPawn->GetActorLocation();

		Direction.Z = 0.0f;

		FRotator TargetRot = Direction.Rotation();

		BossPawn->SetActorRotation(TargetRot);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
