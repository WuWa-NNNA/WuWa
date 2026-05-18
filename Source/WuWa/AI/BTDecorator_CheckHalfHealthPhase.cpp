// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckHalfHealthPhase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckHalfHealthPhase::UBTDecorator_CheckHalfHealthPhase()
{
    NodeName = TEXT("Health <= Threshold OR Is Phase 2");
}

bool UBTDecorator_CheckHalfHealthPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return false;

    float CurrentHealthRatio = BlackboardComp->GetValueAsFloat(HealthRatioKey.SelectedKeyName);
    bool bIsPhase2 = BlackboardComp->GetValueAsBool(IsPhase2Key.SelectedKeyName);

    return (CurrentHealthRatio <= HealthThreshold) || bIsPhase2;
}
