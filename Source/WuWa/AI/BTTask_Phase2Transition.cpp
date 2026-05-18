// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Phase2Transition.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/Sigillum/Sigillum.h"

UBTTask_Phase2Transition::UBTTask_Phase2Transition()
{
    NodeName = TEXT("Phase 2 Transition");
    bNotifyTick = true; 
}

EBTNodeResult::Type UBTTask_Phase2Transition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!AIController || !BlackboardComp) return EBTNodeResult::Failed;

    ASigillum* BossCharacter = Cast<ASigillum>(AIController->GetPawn());
    if (!BossCharacter || !TransitionMontage) return EBTNodeResult::Failed;

    if (BossCharacter && BossCharacter->GetWeapon())
    {
        BossCharacter->GetWeapon()->EmptyOverrideMaterials();
    }

    // BossCharacter 무기 mesh에 모든 엘리먼트 보이도록 해야함.

    BlackboardComp->SetValueAsBool(IsPhase2Key.SelectedKeyName, true);
    BlackboardComp->SetValueAsFloat(HealthRatioKey.SelectedKeyName, 1.0f);

    BossCharacter->PlayAnimMontage(TransitionMontage);

    return EBTNodeResult::InProgress;
}

void UBTTask_Phase2Transition::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ACharacter* BossCharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (BossCharacter && BossCharacter->GetMesh()->GetAnimInstance())
    {
        if (!BossCharacter->GetMesh()->GetAnimInstance()->Montage_IsPlaying(TransitionMontage))
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    }
}
