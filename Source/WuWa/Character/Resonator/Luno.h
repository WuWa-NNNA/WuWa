#pragma once

#include "CoreMinimal.h"
#include "Character/Resonator/Resonator.h"
#include "Luno.generated.h"

UENUM(BlueprintType)
enum class ELunoState : uint8
{
	Normal,
	Half,
	Crescent
};

UCLASS()
class WUWA_API ALuno : public AResonator
{
	GENERATED_BODY()
	
public:
	ALuno();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> WeaponTrail;
};
