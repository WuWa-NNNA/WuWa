// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UUWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UUWorldUserWidget(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

public :
	FORCEINLINE void SetMaxHp(float NewMaxHP) { MaxHp = NewMaxHP; }
	void UpdateHpBar(float NewCurrentHp);

	FORCEINLINE void SetMaxDash(float NewMaxDash) { MaxDash = NewMaxDash; }
	void UpdateDashBar(float NewCurrentDash);

protected :
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	UPROPERTY()
	TObjectPtr<class UProgressBar> DashProgressBar;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float MaxDash;

};
