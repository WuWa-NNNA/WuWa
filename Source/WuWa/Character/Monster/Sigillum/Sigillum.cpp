// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Sigillum/Sigillum.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AIController.h" 
#include "BehaviorTree/BlackboardComponent.h"

#include "Stat/Monster/SigillumStatComponent.h"
#include "UI/UWorldUserWidget.h"

ASigillum::ASigillum(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USigillumStatComponent>(TEXT("Stat")))
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
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


void ASigillum::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(EvadeAndAttackAction, ETriggerEvent::Started, this, &ASigillum::EvadeAndAttack);
		EnhancedInputComponent->BindAction(DiveAttackAction, ETriggerEvent::Started, this, &ASigillum::DiveAttack);
		EnhancedInputComponent->BindAction(ParalysisAction, ETriggerEvent::Started, this, &ASigillum::ChangeToParalysis);
	}
}

float ASigillum::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	AAIController* AICon = Cast<AAIController>(GetController());

	if (AICon && AICon->GetBlackboardComponent())
	{
		AICon->GetBlackboardComponent()->SetValueAsBool(FName("IsStaggered"), true);
	}

	// TODO: DamageCauser와 논의해서 결정 필요

	/*AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon && AICon->GetBlackboardComponent())
	{
		bool bIsHeavyOrParryAttack = CheckIfHeavyAttack(DamageEvent);

		if (bIsHeavyOrParryAttack)
		{
			AICon->GetBlackboardComponent()->SetValueAsBool(FName("IsStaggered"), true);
		}
		else
		{
		}
	}*/

	return ActualDamage;
}

void ASigillum::EvadeAndAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EvadeAndAttackMontage)
	{
		AnimInstance->Montage_Play(EvadeAndAttackMontage);
	}
}
void ASigillum::DiveAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	if (bIsDiveAttacking) return;

	if (AnimInstance && DiveAttackMontage && MovementComp)
	{
		bIsDiveAttacking = true; 
		LaunchCharacter(FVector(0.f, 0.f, 5000.f), true, true);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Dive, this, &ASigillum::PlayAirMontage, 0.3f, false);
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

void ASigillum::PlayAirMontage()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetMesh()->GetAnimInstance()->Montage_Play(DiveAttackMontage);
}

void ASigillum::ResetDiveAttackMovement()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		bIsDiveAttacking = false; 
		MovementComp->SetMovementMode(MOVE_Falling);
	}
}
