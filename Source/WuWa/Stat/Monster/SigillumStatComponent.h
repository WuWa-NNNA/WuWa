// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/Monster/MonsterStatComponent.h"
#include "SigillumStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnParryDelegate, float);

/**
 * 
 */
UCLASS()
class WUWA_API USigillumStatComponent : public UMonsterStatComponent
{
	GENERATED_BODY()
public :
	USigillumStatComponent();


	FOnParryDelegate OnParry;

	virtual void BeginPlay() override;

	FORCEINLINE float GetParryGauge() { return ParryGauge; }
	void SetParryGauge(float ApplyParryGauge);

private :
	float ParryGauge;
	float MaxParryGauge;
};
