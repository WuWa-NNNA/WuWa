// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/WWStatComponent.h"
#include "MonsterStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UMonsterStatComponent : public UWWStatComponent
{
	GENERATED_BODY()

public :
	void showLockOn();
	void hideLockOn();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI_LockOn")
	TSubclassOf<class AActor> AttachActorClass;
	
	UPROPERTY()
	TObjectPtr<AActor> SpawnedLockOn;
};
