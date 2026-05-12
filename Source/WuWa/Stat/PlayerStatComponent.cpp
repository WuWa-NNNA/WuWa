// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHP = 100.f;
	CurrentHP = MaxHP;

	MaxDash = 100.f;
	CurrentDash = MaxDash;
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	SetHp(MaxDash);
	
}

float UPlayerStatComponent::ApplyDamage(float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("ApplyDamage"));
	const float PrevHP = CurrentHP;
	const float ActualDamage = FMath::Clamp<float>(Damage, 0, Damage);
	SetHp(FMath::Clamp<float>(PrevHP - ActualDamage, 0.0f, MaxHP));
	if (CurrentHP <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
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

void UPlayerStatComponent::SetHp(float NewHp)
{
	UE_LOG(LogTemp, Log, TEXT("BroadcastCurrentHP"));

	CurrentHP = FMath::Clamp<float>(NewHp, 0.0f, MaxHP);
	OnHpChagned.Broadcast(CurrentHP);
}

void UPlayerStatComponent::SetDash(float NewDash)
{
	UE_LOG(LogTemp, Log, TEXT("BroadcastCurrentDash"));

	CurrentDash = FMath::Clamp<float>(NewDash, 0.0f, MaxDash);
	OnDashChanged.Broadcast(CurrentDash);
}
