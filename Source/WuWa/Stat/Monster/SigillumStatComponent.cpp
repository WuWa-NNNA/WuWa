// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Monster/SigillumStatComponent.h"

USigillumStatComponent::USigillumStatComponent()
{
	SetParryGauge(1.0f);
}

void USigillumStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USigillumStatComponent::SetParryGauge(float ApplyParryGauge)
{
	ParryGauge = ApplyParryGauge;
	OnParry.Broadcast(ParryGauge);
}

