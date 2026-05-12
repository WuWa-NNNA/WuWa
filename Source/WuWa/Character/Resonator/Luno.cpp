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
