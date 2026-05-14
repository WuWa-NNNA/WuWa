// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WWStatComponent.h"
#include "PlayerStatComponent.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashChangedDelegate, float /*currentDash*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WUWA_API UPlayerStatComponent : public UWWStatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public :
	FOnDashChangedDelegate OnDashChanged;

	FORCEINLINE float GetMaxDash() { return MaxDash; }
	FORCEINLINE float GetCurrentDash() { return CurrentDash; }
	void ApplyDash();

protected:	

	
	void SetDash(float NewDash);

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxDash;
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat")
	float CurrentDash;

};
