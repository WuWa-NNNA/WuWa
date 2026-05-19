// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/Monster/MonsterStatComponent.h"
#include "SigillumStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API USigillumStatComponent : public UMonsterStatComponent
{
	GENERATED_BODY()
public :
	USigillumStatComponent();

	virtual void BeginPlay() override;

};
