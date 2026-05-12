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

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack() override;

	// combo action section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;
};
