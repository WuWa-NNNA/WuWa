#include "Character/Resonator/Luno.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"

#include "Stat/LunoStatComponent.h"

ALuno::ALuno(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<ULunoStatComponent>(TEXT("Stat")))
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

	ChangeLunoState(ELunoState::Half);
}

void ALuno::TickLocomotionGait(float DeltaSeconds)
{
	switch (GetCurrentLocomotionGait())
	{
	case ELocomotionGait::Sprint:
		if (CurrentLunoState == ELunoState::Half && GetVelocity().Length() <= 50.0f)
		{
			ChangeLocomotionGait(ELocomotionGait::Run);
		}
		break;
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

	SetAttackMontage(AttackMontages[NextLunoState]);
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

void ALuno::Dash()
{
	if (CurrentLunoState == ELunoState::Crescent)
	{
		return;
	}

	Super::Dash();
}

void ALuno::Skill()
{
	if (GetCurrentState() == EResonatorState::Attack || CurrentLunoState == ELunoState::Crescent)
	{
		return;
	}

	Super::Skill();

	ChangeLunoState(ELunoState::Crescent);
}
