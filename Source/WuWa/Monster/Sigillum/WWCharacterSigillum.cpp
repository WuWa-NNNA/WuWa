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
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AWWCharacterSigillum::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWWCharacterSigillum::Attack);
	}
}

void AWWCharacterSigillum::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack 실행됨!"));
	ProcessComboCommand();
}

void AWWCharacterSigillum::ProcessComboCommand()
{
    // 1. 내가 바라보는 방향으로 400.f 만큼 떨어진 '절대 좌표' 계산
    FVector ForwardDir = GetActorForwardVector();
    FVector StartLoc = GetActorLocation();

    // 이동하고 싶은 총 거리 (애니메이션의 전진폭에 맞춰 조절)
    float MoveDistance = 5000.f;
    FVector WarpTargetLocation = StartLoc + (ForwardDir * MoveDistance);

    // 2. 모션 워핑 컴포넌트에게 이 좌표로 가라고 명령
    if (MotionWarpingComponent)
    {
        // 몽타주 노티파이의 Warp Target Name과 일치해야 함
        MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("AttackTarget"), WarpTargetLocation);
    }

    // 3. 몽타주 재생
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && ComboActionMontage)
    {
        AnimInstance->Montage_Play(ComboActionMontage);
    }
}
