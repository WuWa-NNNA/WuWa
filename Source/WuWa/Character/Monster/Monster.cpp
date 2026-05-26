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

void AMonster::SetDead()
{
	bIsDead = true;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.0f);
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}

	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AMonster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
	if (bIsBeingParringTiming && AICon && AICon->GetBlackboardComponent())
	{
		AICon->GetBlackboardComponent()->SetValueAsBool(FName("IsStaggered"), true);
	}

	return ActualDamage;
}

void AMonster::DamagedTestBoss()
{
	float CurrentHP = Stat->GetCurrentHP() - 10.f;
	Stat->SetHp(CurrentHP);
}

void AMonster::SetIsParringTiming(bool InIsParringTiming)
{
	bIsBeingParringTiming = InIsParringTiming;
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