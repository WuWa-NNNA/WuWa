// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckTargetSide.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTDecorator_CheckTargetSide::UBTDecorator_CheckTargetSide()
{
    NodeName = TEXT("Check Target Side (Left / Right)");
}

bool UBTDecorator_CheckTargetSide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AAIController* AIController = OwnerComp.GetAIOwner();

    if (!BlackboardComp || !AIController) return false;

    APawn* BossPawn = AIController->GetPawn();
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

    if (!BossPawn || !TargetActor) return false;

    FVector BossRightVector = BossPawn->GetActorRightVector();

    FVector DirectionToTarget = (TargetActor->GetActorLocation() - BossPawn->GetActorLocation()).GetSafeNormal();

    float DotResult = FVector::DotProduct(BossRightVector, DirectionToTarget);

    if (SideToCheck == ETargetSide::Right)
    {
        return DotResult > 0.0f; 
    }
    else
    {
        return DotResult <= 0.0f;
    }
}
