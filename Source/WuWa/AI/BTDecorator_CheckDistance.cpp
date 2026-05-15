// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
    NodeName = TEXT("Check Distance");
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return false;

    uint8 CurrentDistanceVal = BlackboardComp->GetValueAsEnum(DistanceKey.SelectedKeyName);
    EDistanceRange CurrentDistance = static_cast<EDistanceRange>(CurrentDistanceVal);

    return AllowedDistances.Contains(CurrentDistance);
}
