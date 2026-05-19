// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/WWAIController.h"
#include "SigillumAIController.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API ASigillumAIController : public AWWAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void HandleHPChanged(float CurrentHP);

};
