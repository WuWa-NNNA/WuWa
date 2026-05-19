// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WWAIController.generated.h"

class UBlackboardData;
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
	FORCEINLINE UBlackboardData* GetBBAsset() { return BBAsset; }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

protected:

	UPROPERTY()
	TObjectPtr<class UWWStatComponent> CachedStatComp;

	void FindPlayerAndSetTarget();
	FTimerHandle TimerHandle_FindPlayer;

	UPROPERTY(EditAnywhere, category = "Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, category = "Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
