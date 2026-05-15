// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TeleportNearPlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h" 

UBTTask_TeleportNearPlayer::UBTTask_TeleportNearPlayer()
{
    NodeName = TEXT("Teleport Near Player");
}

EBTNodeResult::Type UBTTask_TeleportNearPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ACharacter* BossCharacter = Cast<ACharacter>(AIController->GetPawn());
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!BossCharacter || !PlayerCharacter) return EBTNodeResult::Failed;

    FVector PlayerLoc = PlayerCharacter->GetActorLocation();
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    FVector TargetLocation = PlayerLoc;

    if (NavSys)
    {
        float MinDistance = 150.0f; // 최소 간격 (1.5m)
        float MaxDistance = 300.0f; // 최대 간격 (3.0m)

        FVector RandomDirection = FMath::VRand();
        RandomDirection.Z = 0.0f;
        RandomDirection.Normalize();

        float RandomDistance = FMath::RandRange(MinDistance, MaxDistance);

        FVector DesiredLocation = PlayerLoc + (RandomDirection * RandomDistance);

        FNavLocation ProjectedLocation;
        if (NavSys->ProjectPointToNavigation(DesiredLocation, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
        {
            TargetLocation = ProjectedLocation.Location;
        }
    }

    BossCharacter->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, PlayerLoc);
    LookAtRot.Pitch = 0.0f;
    LookAtRot.Roll = 0.0f;

    BossCharacter->SetActorRotation(LookAtRot);
    AIController->SetControlRotation(LookAtRot);

    return EBTNodeResult::Succeeded;
}