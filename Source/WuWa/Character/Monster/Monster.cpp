// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Stat/Monster/MonsterStatComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AMonster::AMonster(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMonsterStatComponent>(TEXT("Stat")))
{
	TeamType = ETeamType::Monster;

	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->GravityScale = 1.5f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponProp05"));
}

void AMonster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMonster::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMonster::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMonster::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMonster::Attack);
	}

}

void AMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AMonster::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector NewMoveInputDirection = ForwardVector * Movement.Y + RightVector * Movement.X;
	if (NewMoveInputDirection.IsNearlyZero())
	{
		return;
	}

	CurrentMoveInputDirection = NewMoveInputDirection.GetSafeNormal();
	AddMovementInput(CurrentMoveInputDirection);

}

void AMonster::Look(const FInputActionValue& Value)
{
	FVector2D RotationValue = Value.Get<FVector2D>();

	AddControllerYawInput(RotationValue.X);
	AddControllerPitchInput(-RotationValue.Y);
}


void AMonster::Jump()
{

	if (GetVelocity().Length() > 50.0f)
	{
		PlayAnimMontage(JumpMontage);
	}
}

void AMonster::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	UMonsterStatComponent* MonsterStat = Cast<UMonsterStatComponent>(Stat);
	FActorSpawnParameters Params;
	Params.Owner = this;

	MonsterStat->SpawnedLockOn = GetWorld()->SpawnActor<AActor>(MonsterStat->AttachActorClass);

	if (IsValid(MonsterStat->SpawnedLockOn))
	{
		MonsterStat->SpawnedLockOn->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("Bip001Spine2")
		);		
		hideLockOnMonster();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No SpawnedLockOn"));
	}
}


float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AMonster::DamagedTestBoss()
{
	float CurrentHP = Stat->GetCurrentHP() - 10.f;
	Stat->SetHp(CurrentHP);
}
void AMonster::showLockOnMonster()
{
	UMonsterStatComponent* MonsterStat = Cast<UMonsterStatComponent>(Stat);
	if (!MonsterStat)
	{
		return;
	}
	MonsterStat->showLockOn();

}
void AMonster::hideLockOnMonster()
{
	UMonsterStatComponent* MonsterStat = Cast<UMonsterStatComponent>(Stat);
	if (!MonsterStat)
	{
		return;
	}
	MonsterStat->hideLockOn();

}