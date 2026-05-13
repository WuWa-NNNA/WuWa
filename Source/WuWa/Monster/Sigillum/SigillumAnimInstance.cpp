// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Sigillum/SigillumAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/Sigillum/Sigillum.h"

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
	}
}

void USigillumAnimInstance::AnimNotify_AN_EndDiveAttack()
{
	if (Movement)
	{
		Movement->SetMovementMode(MOVE_Falling);
	}

	if (Owner)
	{
		Owner->ResetDiveAttackMovement();
	}
}
