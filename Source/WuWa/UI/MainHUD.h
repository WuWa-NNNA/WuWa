// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UMainHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

protected :
	//UPROPERTY()
	//TObjectPtr<clas


};
