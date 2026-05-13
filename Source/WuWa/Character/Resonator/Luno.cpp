#include "Character/Resonator/Luno.h"

#include "NiagaraComponent.h"

ALuno::ALuno()
{
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

	ChangeLunoState(ELunoState::Normal);
}

void ALuno::ChangeLunoState(ELunoState NextLunoState)
{
	SetAttackMontage(AttackMontages[NextLunoState]);
	SetAttackComboData(AttackComboDatas[NextLunoState]);
	CurrentLunoState = NextLunoState;
}

void ALuno::SAttack()
{
	if (GetCurrentState() != EResonatorState::Normal)
	{
		return;
	}

	Super::SAttack();

	switch (CurrentLunoState)
	{
	case ELunoState::Normal:
		ChangeLunoState(ELunoState::Crescent);
		break;
	case ELunoState::Crescent:
		ChangeLunoState(ELunoState::Normal);
		break;
	}
}
