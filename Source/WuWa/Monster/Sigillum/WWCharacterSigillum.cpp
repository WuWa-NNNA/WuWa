// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Sigillum/WWCharacterSigillum.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

AWWCharacterSigillum::AWWCharacterSigillum()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
}

void AWWCharacterSigillum::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWWCharacterSigillum::Attack()
{
	Super::Attack();
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && ComboActionMontage)
    {
        AnimInstance->Montage_Play(ComboActionMontage);
    }
}
