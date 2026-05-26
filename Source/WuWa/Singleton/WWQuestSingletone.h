// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/WWQuestData.h"
#include "WWQuestSingletone.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UWWQuestSingletone : public UObject
{
	GENERATED_BODY()

public :
	UWWQuestSingletone();
	
	static UWWQuestSingletone& Get();


public :
	FORCEINLINE FWWQuestData GetQuestData(int32 InLevel) { return QuestDataTable.IsValidIndex(InLevel) ? QuestDataTable[InLevel]: FWWQuestData(); }

	UPROPERTY()
	int32 QuestDataMaxLevel;

private :
	TArray<FWWQuestData> QuestDataTable;
	
};
