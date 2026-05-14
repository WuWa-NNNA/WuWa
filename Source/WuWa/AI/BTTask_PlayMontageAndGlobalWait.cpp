// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PlayMontageAndGlobalWait.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTask_PlayMontageAndGlobalWait::UBTTask_PlayMontageAndGlobalWait()
{
    NodeName = TEXT("Play Boss Montage");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayMontageAndGlobalWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ACharacter* BossCharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!BossCharacter || !MontageToPlay) return EBTNodeResult::Failed;

    BossCharacter->PlayAnimMontage(MontageToPlay);

    return EBTNodeResult::InProgress;
}

void UBTTask_PlayMontageAndGlobalWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ACharacter* BossCharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (BossCharacter && BossCharacter->GetMesh()->GetAnimInstance())
    {
        if (!BossCharacter->GetMesh()->GetAnimInstance()->Montage_IsPlaying(MontageToPlay))
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    }
}
