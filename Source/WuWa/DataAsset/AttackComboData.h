// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackComboData.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UAttackComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	uint32 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray<float> EffectiveFrameCount;
};
