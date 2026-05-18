// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckGlobalCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"

UBTDecorator_CheckGlobalCooldown::UBTDecorator_CheckGlobalCooldown()
{
    NodeName = TEXT("Check Global Cooldown");
}

bool UBTDecorator_CheckGlobalCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    UWorld* World = OwnerComp.GetWorld();

    if (!BlackboardComp || !World) return false;

    float CurrentTime = World->GetTimeSeconds();

    float NextAttackTime = BlackboardComp->GetValueAsFloat(NextAttackTimeKey.SelectedKeyName);

    return CurrentTime >= NextAttackTime;
}
