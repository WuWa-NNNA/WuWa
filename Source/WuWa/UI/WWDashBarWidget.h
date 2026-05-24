// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWDashBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UWWDashBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	virtual void NativeConstruct() override;

	void UpdataWorldDash(float NewCurrentDash);
	void HideWorldDash();

protected :

	UPROPERTY()
	TObjectPtr<class UImage> DashGaugeImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DashGaugeMaterial;

	float MaxDash;
};
