// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*currentHP*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashChangedDelegate, float /*currentDash*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WUWA_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public :
	FOnHPZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChagned;
	
	FOnDashChangedDelegate OnDashChanged;

	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurrentHP() { return CurrentHP; }
	float ApplyDamage(float Damage);

	FORCEINLINE float GetMaxDash() { return MaxHP; }
	FORCEINLINE float GetCurrentDash() { return CurrentHP; }
	float ApplyDash();

protected:	

	void SetHp(float NewHp);
	void SetDash(float NewDash);

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxDash;
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat")
	float CurrentDash;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxHP;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat")
	float CurrentHP;

};
