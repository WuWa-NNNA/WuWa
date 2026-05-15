// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/WWStatComponent.h"

UWWStatComponent::UWWStatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	MaxHP = 100.f;
	CurrentHP = MaxHP;
	SetLevel(90);
}


// Called when the game starts
void UWWStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	SetHp(MaxHP);
}


float UWWStatComponent::ApplyDamage(float Damage)
{
	//UE_LOG(LogTemp, Log, TEXT("ApplyDamage"));
	const float PrevHP = CurrentHP;
	const float ActualDamage = FMath::Clamp<float>(Damage, 0, Damage);
	SetHp(FMath::Clamp<float>(PrevHP - ActualDamage, 0.0f, MaxHP));
	if (CurrentHP <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UWWStatComponent::SetHp(float NewHp)
{
	//UE_LOG(LogTemp, Log, TEXT("BroadcastCurrentHP"));

	CurrentHP = FMath::Clamp<float>(NewHp, 0.0f, MaxHP);
	OnHpChagned.Broadcast(CurrentHP);
}