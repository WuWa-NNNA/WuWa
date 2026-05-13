// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"

#include "Sigillum.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API ASigillum : public AMonster
{
	GENERATED_BODY()
	
public:
	ASigillum();
	void ResetDiveAttackMovement();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void EvadeAndAttack();
	void DiveAttack();

	// 마비상태 테스트 함수 만들기.

private: // dive
	bool bIsDiveAttacking = false;
	FTimerHandle TimerHandle_Dive;
	void PlayAirMontage();

private: // inputaction
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EvadeAndAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DiveAttackAction;

private: // montage;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> EvadeAndAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DiveAttackMontage;
};
