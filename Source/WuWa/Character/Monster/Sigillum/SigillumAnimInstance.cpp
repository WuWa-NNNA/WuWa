// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Sigillum/SigillumAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Monster/Sigillum/Sigillum.h"

USigillumAnimInstance::USigillumAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 100.f;
}

void USigillumAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ASigillum>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void USigillumAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling;
		bIsDeath = Owner->bIsDead;
	}
}



void USigillumAnimInstance::AnimNotify_AN_CheckParalysis()
{
	if (ParalysisState.Cycle <= 0) return;

	ParalysisState.Cycle--;

	if (ParalysisState.Cycle == 0)
	{
		if (Owner && Owner->GetParalysisMontage())
		{
			Montage_SetNextSection(FName("ParalysisLoop"), FName("ParalysisEnd"), Owner->GetParalysisMontage());
		}
	}
}