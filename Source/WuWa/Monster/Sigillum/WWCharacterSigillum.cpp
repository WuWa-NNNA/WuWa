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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWWCharacterSigillum::Attack);
	}
}

void AWWCharacterSigillum::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack 실행됨!"));
	ProcessComboCommand();
}

void AWWCharacterSigillum::ProcessComboCommand()
{
    // 3. 몽타주 재생
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && ComboActionMontage)
    {
        AnimInstance->Montage_Play(ComboActionMontage);
    }
}
