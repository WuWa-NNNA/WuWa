// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMeshVisibility.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTask_SetMeshVisibility::UBTTask_SetMeshVisibility()
{
    NodeName = TEXT("Set Mesh Visibility");
    bHideMesh = true; 
}

EBTNodeResult::Type UBTTask_SetMeshVisibility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ACharacter* BossCharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!BossCharacter || !BossCharacter->GetMesh()) return EBTNodeResult::Failed;

    BossCharacter->SetActorHiddenInGame(bHideMesh);

    return EBTNodeResult::Succeeded;
}
