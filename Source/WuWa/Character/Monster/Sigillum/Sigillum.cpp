// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Sigillum/Sigillum.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Stat/Monster/SigillumStatComponent.h"
#include "UI/UWorldUserWidget.h"

ASigillum::ASigillum(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USigillumStatComponent>(TEXT("Stat")))
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	Stat->SetMaxHP(5000);

	USigillumStatComponent* SigillumStat = Cast<USigillumStatComponent>(Stat);

	if (SigillumStat)
	{
		SigillumStat->SetParryGauge(1.f);
	}
}


void ASigillum::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Weapon && GetMesh())
	{
		Weapon->SetLeaderPoseComponent(GetMesh());
	}

	if (HiddenMaterial && Weapon)
	{
		Weapon->SetMaterial(2, HiddenMaterial);
		Weapon->SetMaterial(3, HiddenMaterial);
		Weapon->SetMaterial(4, HiddenMaterial);
	}
}

bool ASigillum::CanPlayHitReaction() const
{
	USigillumStatComponent* SigillumStat = Cast<USigillumStatComponent>(Stat);
	return SigillumStat && SigillumStat->GetParryGauge() > 0.f;
}

void ASigillum::ChangeToParalysis()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ParalysisMontage)
	{
		AnimInstance->Montage_Play(ParalysisMontage);
	}
}

float ASigillum::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	USigillumStatComponent* SigillumStat = Cast<USigillumStatComponent>(Stat);
	AAIController* AICon = Cast<AAIController>(GetController());
	
	if (bIsBeingParringTiming)
	{
		float Gauge = FMath::Clamp(SigillumStat->GetParryGauge() - 0.1f, 0.f, 1.f);
		SigillumStat->SetParryGauge(Gauge);

		if (Gauge == 0)
		{
			if (!(AICon->GetBlackboardComponent()->GetValueAsBool(FName("IsParalysis"))) && AICon && AICon->GetBlackboardComponent())
			{
				AICon->GetBlackboardComponent()->SetValueAsBool(FName("IsParalysis"), true);
			}
		}
	}

	bool IsSuperDamageTime = SigillumStat->GetParryGauge() == 0;
	float Damage = Super::TakeDamage(IsSuperDamageTime ? DamageAmount * 100 : DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}
