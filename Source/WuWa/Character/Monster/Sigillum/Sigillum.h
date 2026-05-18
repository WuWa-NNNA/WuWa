// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/Monster.h"

#include "Sigillum.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API ASigillum : public AMonster
{
	GENERATED_BODY()
	
public:
	ASigillum(const FObjectInitializer& ObjectInitializer);
	void ResetDiveAttackMovement();
	virtual void OnConstruction(const FTransform& Transform) override;
	UAnimMontage* GetParalysisMontage() const { return ParalysisMontage; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void EvadeAndAttack();
	void DiveAttack();
	void ChangeToParalysis();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInterface* HiddenMaterial;

private: // dive
	bool bIsDiveAttacking = false;
	FTimerHandle TimerHandle_Dive;
	void PlayAirMontage();

private: // inputaction
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EvadeAndAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DiveAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ParalysisAction;

private: // montage;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> EvadeAndAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DiveAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ParalysisMontage;

};
