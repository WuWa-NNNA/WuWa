// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxDash = 100.f;
	CurrentDash = MaxDash;
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetDash(MaxDash);
}



float UPlayerStatComponent::ApplyDash()
{
	UE_LOG(LogTemp, Log, TEXT("ApplyDash"));

	const float PrevHP = CurrentDash;
	if (CurrentDash <= 0.9f)
	{
		return -1;
	}
	CurrentDash--;
	SetDash(CurrentDash);
	return 1;
}



void UPlayerStatComponent::SetDash(float NewDash)
{
	UE_LOG(LogTemp, Log, TEXT("BroadcastCurrentDash"));

	CurrentDash = FMath::Clamp<float>(NewDash, 0.0f, MaxDash);
	OnDashChanged.Broadcast(CurrentDash);
}
