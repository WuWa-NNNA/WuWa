// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Sigillum/Sigillum.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

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
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	USigillumStatComponent* SigillumStat = Cast<USigillumStatComponent>(Stat);

	if (bIsBeingParringTiming)
	{
		SigillumStat->SetParryGauge(FMath::Clamp(SigillumStat->GetParryGauge() - 0.1f, 0.f, 1.f));
	}
	return Damage;
}
