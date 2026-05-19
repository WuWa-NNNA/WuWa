// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WWStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*currentHP*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WUWA_API UWWStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWWStatComponent();
	FOnHPZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChagned;

	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float SetMaxHP(float hp) { return MaxHP = hp; }

	FORCEINLINE float GetCurrentHP() { return CurrentHP; }
	float ApplyDamage(float Damage);

	FORCEINLINE int GetLevel() { return Level; }
	FORCEINLINE int SetLevel(int levelup) { return Level = levelup; }

	void SetHp(float NewHp);


protected:

	// Called when the game starts¤Ñ,.
	
	virtual void BeginPlay() override;


	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxHP;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat")
	float CurrentHP;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	int Level;


};
