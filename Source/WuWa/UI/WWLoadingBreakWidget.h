// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWLoadingBreakWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UWWLoadingBreakWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:

	UFUNCTION(BlueprintCallable)
	void StartBreak();

	UFUNCTION(BlueprintCallable)
	void UpdateBreak();

protected :
	UPROPERTY()
	TObjectPtr<class UImage> LoadingImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> LoadingImageaterial;

private :
	FTimerHandle BreakTimerHandle;

	float CurrentProgress;

};
