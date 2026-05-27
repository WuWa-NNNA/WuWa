// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWTouchWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UWWTouchWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	void ChangeImageColor(FLinearColor color);

protected :

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image4;
};
