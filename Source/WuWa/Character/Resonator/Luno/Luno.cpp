#include "Character/Resonator/Luno/Luno.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Stat/Player/LunoStatComponent.h"

ALuno::ALuno(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULunoStatComponent>(TEXT("Stat")))
{
	Bead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bead"));
	Bead->SetupAttachment(GetMesh(), TEXT("WeaponProp07"));
	Bead->SetVisibility(false);

	BeadLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("BeadLight"));
	BeadLight->SetupAttachment(Bead);
	BeadLight->SetIntensity(35.0f);
	BeadLight->SetAttenuationRadius(200.0f);
	BeadLight->SetVisibility(false);

	ArrowStartEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ArrowStartEffect1"));
	ArrowStartEffect1->SetupAttachment(GetMesh(), TEXT("ArrowStart"));
	ArrowStartEffect1->SetVariableBool(TEXT("User.AudioOn"), false);
	ArrowStartEffect1->SetVariableBool(TEXT("User.NoLaunchVFX"), false);
	ArrowStartEffect1->SetVariableFloat(TEXT("User.ColorHue"), 0.85f);
	ArrowStartEffect1->SetVariableFloat(TEXT("User._Size"), 1.0f);
	ArrowStartEffect1->SetVisibility(false);

	ArrowStartEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ArrowStartEffect2"));
	ArrowStartEffect2->SetupAttachment(GetMesh(), TEXT("ArrowStart"));
	ArrowStartEffect2->SetVariableBool(TEXT("User.AudioOn"), false);
	ArrowStartEffect2->SetVariableBool(TEXT("User.NoLaunchVFX"), false);
	ArrowStartEffect2->SetVariableFloat(TEXT("User.ColorHue"), 0.85f);
	ArrowStartEffect2->SetVariableFloat(TEXT("User._Size"), 1.0f);
	ArrowStartEffect2->SetVisibility(false);

	WeaponTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponTrail"));
	WeaponTrail->SetupAttachment(GetWeaponMeshComponent());
	WeaponTrail->SetVariableBool(TEXT("User.AudioOn"), false);
	WeaponTrail->SetVariableFloat(TEXT("User._ColorHue"), 0.35f);
	WeaponTrail->SetVariableFloat(TEXT("User._Size"), 1.5f);
	WeaponTrail->SetVisibility(false);
}

void ALuno::BeginPlay()
{
	Super::BeginPlay();

	ChangeLunoState(ELunoState::Half);
}

void ALuno::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentLunoState == ELunoState::Crescent && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying)
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	}
}

void ALuno::ChangeLunoState(ELunoState NextLunoState)
{
	switch (CurrentLunoState)
	{
	case ELunoState::Half:
		break;
	case ELunoState::Crescent:
		GetWeaponMeshComponent()->SetVisibility(false);
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		break;
	}

	switch (NextLunoState)
	{
	case ELunoState::Half:
		ChangeState(EResonatorState::Normal);
		GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
		GetWeaponMeshComponent()->GetAnimInstance()->StopAllMontages(0.0f);
		break;
	case ELunoState::Crescent:
		GetWeaponMeshComponent()->SetVisibility(true);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Cast<ULunoStatComponent>(Stat)->SetCrescentTimer();
		break;
	}

	GetMesh()->SetMaterial(7, ShoesMaterials[NextLunoState]);
	SetWeaponMaterial(NextLunoState);
	SetDashMontage(DashMontages[NextLunoState]);
	SetDodgeMontage(DodgeMontages[NextLunoState]);
	SetAttackMontage(AttackMontages[NextLunoState]);
	SetWeaponAttackMontage(WeaponAttackMontages[NextLunoState]);
	SetAttackComboData(AttackComboDatas[NextLunoState]);

	CurrentLunoState = NextLunoState;

	if (ULunoStatComponent* LunoStat = Cast<ULunoStatComponent>(Stat))
	{
		LunoStat->ChangeAttackMode(NextLunoState);
		LunoStat->ChangeSkillIcon(GetCurrentAttackCombo());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ChangeLunoState failed"));
	}
}

void ALuno::SetWeaponMaterial(ELunoState NextLunoState)
{
	GetWeaponMeshComponent()->SetMaterial(2, WeaponMaterials[NextLunoState]);
}

FRotator ALuno::GetLockOnRotator() const
{
	return (GetLockOnTarget()->GetActorLocation() - GetMesh()->GetSocketLocation(TEXT("ArrowStart"))).Rotation();
}

void ALuno::Attack()
{
	if (CurrentLunoState == ELunoState::Half)
	{
		Super::Attack();
		return;
	}

	ProcessAttack();
}

void ALuno::Skill()
{
	if (CurrentLunoState == ELunoState::Crescent)
	{
		return;
	}

	Super::Skill();
}

void ALuno::PlayDashMontage()
{
	if (CurrentLunoState == ELunoState::Half)
	{
		Super::PlayDashMontage();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* WeaponAnimInstance = GetWeaponMeshComponent()->GetAnimInstance();

	UAnimMontage* Montage = GetDashMontage();
	UAnimMontage* WeaponMotage = GetWeaponDashMontage();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnDashMontageEnded);
	if (HasCurrentMoveInput())
	{
		PlayAnimMontage(Montage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
		WeaponAnimInstance->Montage_Play(WeaponMotage, 1.5f);
	}
	else
	{
		PlayAnimMontage(Montage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
		AnimInstance->Montage_JumpToSection(TEXT("Back"), Montage);
		WeaponAnimInstance->Montage_Play(WeaponMotage, 1.5f);
	}
}

void ALuno::PlayDodgeMontage()
{
	if (CurrentLunoState == ELunoState::Half)
	{
		Super::PlayDodgeMontage();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* WeaponAnimInstance = GetWeaponMeshComponent()->GetAnimInstance();

	UAnimMontage* Montage = GetDodgeMontage();
	UAnimMontage* WeaponMotage = GetWeaponDashMontage();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnDodgeMontageEnded);
	if (HasCurrentMoveInput())
	{
		PlayAnimMontage(Montage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
		WeaponAnimInstance->Montage_Play(WeaponMotage, 1.5f);
	}
	else
	{
		PlayAnimMontage(Montage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
		AnimInstance->Montage_JumpToSection(TEXT("Back"), Montage);
		WeaponAnimInstance->Montage_Play(WeaponMotage, 1.5f);
	}
}

void ALuno::SpawnAttackHitEffect(const FHitResult& HitResult)
{
	if (AttackHitEffect)
	{
		UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
		SpawnedEffect->SetVariableFloat(TEXT("User._Size"), 2.0f);
	}
}
