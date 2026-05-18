#include "Character/Resonator/Luno/Luno.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "NiagaraComponent.h"

#include "Stat/LunoStatComponent.h"

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

void ALuno::ChangeLunoState(ELunoState NextLunoState)
{
	switch (CurrentLunoState)
	{
	case ELunoState::Half:
		break;
	case ELunoState::Crescent:
		GetWeaponMeshComponent()->SetVisibility(false);
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		break;
	}

	switch (NextLunoState)
	{
	case ELunoState::Half:
		break;
	case ELunoState::Crescent:
		GetWeaponMeshComponent()->SetVisibility(true);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		break;
	}

	GetMesh()->SetMaterial(7, ShoesMaterials[NextLunoState]);
	SetWeaponMaterial(NextLunoState);
	SetDashMontage(DashMontages[NextLunoState]);
	SetAttackMontage(AttackMontages[NextLunoState]);
	SetWeaponAttackMontage(WeaponAttackMontages[NextLunoState]);
	SetAttackComboData(AttackComboDatas[NextLunoState]);

	CurrentLunoState = NextLunoState;

	if (ULunoStatComponent* LunoStat = Cast<ULunoStatComponent>(Stat))
	{
		UE_LOG(LogTemp, Log, TEXT("ChangeLunoState succes"));
		LunoStat->ChangeAttackMode(NextLunoState);
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

	UAnimMontage* Motage = GetDashMontage();
	UAnimMontage* WeaponMotage = GetWeaponDashMontage();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnDashMontageEnded);
	if (HasCurrentMoveInput())
	{
		PlayAnimMontage(Motage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Motage);
		WeaponAnimInstance->Montage_Play(WeaponMotage, 1.5f);
	}
	else
	{
		PlayAnimMontage(Motage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Motage);
		AnimInstance->Montage_JumpToSection(TEXT("Back"), Motage);
		WeaponAnimInstance->Montage_Play(WeaponMotage, 1.5f);
	}
}
