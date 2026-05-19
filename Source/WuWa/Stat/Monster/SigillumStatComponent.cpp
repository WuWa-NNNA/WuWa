// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Monster/SigillumStatComponent.h"

USigillumStatComponent::USigillumStatComponent()
{
}

void USigillumStatComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("현재 HP : %f"), CurrentHP);

}

