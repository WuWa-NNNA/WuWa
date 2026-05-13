// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WWAIController.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API AWWAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWWAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere, category = "Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, category = "Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
