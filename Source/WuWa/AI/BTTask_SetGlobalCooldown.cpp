// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetGlobalCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"

UBTTask_SetGlobalCooldown::UBTTask_SetGlobalCooldown()
{
    NodeName = TEXT("Set Global Cooldown");
}

EBTNodeResult::Type UBTTask_SetGlobalCooldown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    UWorld* World = OwnerComp.GetWorld();

    if (!BlackboardComp || !World) return EBTNodeResult::Failed;

    float NextTime = World->GetTimeSeconds() + CooldownTime;
    BlackboardComp->SetValueAsFloat(NextAttackTimeKey.SelectedKeyName, NextTime);

    return EBTNodeResult::Succeeded;
}
