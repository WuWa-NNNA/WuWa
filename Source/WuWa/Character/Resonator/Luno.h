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
	virtual void BeginPlay() override;

public:
	void ChangeLunoState(ELunoState NextLunoState);

protected:
	virtual void SAttack() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonator", meta = (AllowPrivateAccess = "true"))
	ELunoState CurrentLunoState;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> WeaponTrail;

private:
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAnimMontage>> AttackMontages;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAttackComboData>> AttackComboDatas;
};
