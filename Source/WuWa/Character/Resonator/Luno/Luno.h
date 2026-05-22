#pragma once

#include "CoreMinimal.h"
#include "Character/Resonator/Resonator.h"
#include "Luno.generated.h"

UENUM(BlueprintType)
enum class ELunoState : uint8
{
	Half,
	Crescent
};

UCLASS()
class WUWA_API ALuno : public AResonator
{
	GENERATED_BODY()
	
public:
	ALuno(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLunoState(ELunoState NextLunoState);

	UFUNCTION(BlueprintCallable)
	void SetWeaponMaterial(ELunoState NextLunoState);

public:
	virtual FRotator GetLockOnRotator() const override;

protected:
	virtual void Attack() override;
	virtual void Skill() override;

protected:
	virtual void PlayDashMontage() override;
	virtual void PlayDodgeMontage() override;

protected:
	virtual void SpawnAttackHitEffect(const FHitResult& HitResult) override;

public:
	FORCEINLINE ELunoState GetCurrentLunoState() const { return CurrentLunoState; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonator", meta = (AllowPrivateAccess = "true"))
	ELunoState CurrentLunoState;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Bead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPointLightComponent> BeadLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> ArrowStartEffect1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> ArrowStartEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> WeaponTrail;

private:
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAnimMontage>> DashMontages;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAnimMontage>> DodgeMontages;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAnimMontage>> AttackMontages;

private:
	UPROPERTY(EditAnywhere, Category = "WeaponMontage", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAnimMontage>> WeaponAttackMontages;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UAttackComboData>> AttackComboDatas;

private:
	UPROPERTY(EditAnywhere, Category = "Material", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UMaterialInterface>> ShoesMaterials;

	UPROPERTY(EditAnywhere, Category = "Material", meta = (AllowPrivateAccess = "true"))
	TMap<ELunoState, TObjectPtr<class UMaterialInterface>> WeaponMaterials;
};
