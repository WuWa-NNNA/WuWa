// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/WWCharacterMonster.h"

#include "WWCharacterSigillum.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API AWWCharacterSigillum : public AWWCharacterMonster
{
	GENERATED_BODY()
	
public:
	AWWCharacterSigillum();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Attack();

	//UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> AttackAction;

	// combo action section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	void ProcessComboCommand();
};
