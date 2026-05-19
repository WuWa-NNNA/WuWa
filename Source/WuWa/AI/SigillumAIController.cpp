// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SigillumAIController.h"
#include "AI/EDistanceRange.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stat/WWStatComponent.h"
#include "Character/Monster/Sigillum/Sigillum.h"

void ASigillumAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (CachedStatComp)
	{
		CachedStatComp->OnHpChagned.RemoveAll(this);
		CachedStatComp->OnHpChagned.AddUObject(this, &ASigillumAIController::HandleHPChanged);
	}
}
void ASigillumAIController::HandleHPChanged(float CurrentHP)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	ASigillum* SigillumPawn = Cast<ASigillum>(GetPawn());
	if (!BB || !CachedStatComp || !SigillumPawn) return;

	float MaxHP = CachedStatComp->GetMaxHP();
	BB->SetValueAsFloat("HealthRatio", CurrentHP / MaxHP);

	if (CurrentHP <= 0.f)
	{
		bool bIsPhase2 = BB->GetValueAsBool("IsPhase2");

		if (!bIsPhase2)
		{
			
		}
		else
		{
			SigillumPawn->SetDead();
			StopAI();
		}
	}
}