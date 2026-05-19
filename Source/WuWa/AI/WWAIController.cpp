// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WWAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Stat/WWStatComponent.h"

AWWAIController::AWWAIController()
{
}

void AWWAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_FindPlayer, this, &AWWAIController::FindPlayerAndSetTarget, 0.5f, false);
}

void AWWAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AWWAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AWWAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CachedStatComp = InPawn->FindComponentByClass<UWWStatComponent>();

	RunAI();
}

void AWWAIController::OnUnPossess()
{
	if (CachedStatComp)
	{
		CachedStatComp->OnHpChagned.RemoveAll(this);
		CachedStatComp = nullptr;
	}

	Super::OnUnPossess();
}

void AWWAIController::FindPlayerAndSetTarget()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (BlackboardPtr)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

		if (PlayerCharacter)
		{
			BlackboardPtr->SetValueAsObject(FName("Target"), PlayerCharacter);
		}
	}
}
