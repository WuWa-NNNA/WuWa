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

    FName TargetSection = NAME_None;
    int32 ArraySize = SectionNames.Num();

    if (ArraySize == 0)
    {
        TArray<FName> ExtractedSectionNames;
        if (MontageToPlay)
        {
            for (const FCompositeSection& Section : MontageToPlay->CompositeSections)
            {
                ExtractedSectionNames.Add(Section.SectionName);
            }
        }
        TargetSection = ExtractedSectionNames[FMath::RandRange(0, ExtractedSectionNames.Num() - 1)];
    } 
    else if (ArraySize == 1)
    {
        TargetSection = SectionNames[0];
    }
    else if (ArraySize >= 2)
    {
        TargetSection = SectionNames[FMath::RandRange(0, ArraySize - 1)];
    }

    BossCharacter->PlayAnimMontage(MontageToPlay, 1.0f, TargetSection);

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
