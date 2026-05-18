// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CalculateDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("Calculate Distance Range");
	Interval = 0.2f; 
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BlackboardComp || !AIController) return;

	APawn* BossPawn = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (BossPawn && TargetActor)
	{
		float Distance = FVector::Dist(BossPawn->GetActorLocation(), TargetActor->GetActorLocation());
		EDistanceRange CurrentRange = EDistanceRange::Long; 

		if (Distance <= 200.0f) CurrentRange = EDistanceRange::Melee; 
		else if (Distance <= 500.0f) CurrentRange = EDistanceRange::Short; 
		else if (Distance <= 1000.0f) CurrentRange = EDistanceRange::Mid;
		else CurrentRange = EDistanceRange::Long;

		BlackboardComp->SetValueAsEnum(DistanceRangeKey.SelectedKeyName, static_cast<uint8>(CurrentRange));
	}
}