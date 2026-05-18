// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWMonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UWWMonsterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	void BindStatComponent(UWWMonsterWidget* statComponent);

	void UpdateHpBar(float CurrentHp);

protected :
	//UPROPERTY()
	//class UPRogressBar* MonsterHpBar;

};
