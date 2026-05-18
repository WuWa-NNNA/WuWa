// Fill out your copyright notice in the Description page of Project Settings.


#include "WWQuestSingletone.h"
#include "QuestData/WWQuestData.h"
#include "Algo/Transform.h"

UWWQuestSingletone::UWWQuestSingletone()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> QuestdataTableRef(TEXT("/Game/PCH/Asset/GameData/WWQuestData.WWQuestData"));
		
	if(!QuestdataTableRef.Object)
	{
		const UDataTable* LoadedTable = QuestdataTableRef.Object;
		
		if (LoadedTable && LoadedTable->GetRowMap().Num() > 0)
		{
			TArray<uint8*> ValueArray;
			LoadedTable->GetRowMap().GenerateValueArray(ValueArray);
			QuestDataTable.Empty();
			Algo::Transform(ValueArray, QuestDataTable, [](uint8* value)
				{
					return *reinterpret_cast<FWWQuestData*>(value);
				});
		}
		QuestDataMaxLevel = QuestDataTable.Num();
	}
}

UWWQuestSingletone& UWWQuestSingletone::Get()
{
	
	UWWQuestSingletone* singleton = Cast<UWWQuestSingletone>(GEngine->GameSingleton);
	if (singleton)
	{
		return *singleton;
	}
	UE_LOG(LogTemp, Error, TEXT("GameSingleton is NULL! Please check Project Settings > Engine > General Settings > Game Singleton Class."));
	static UWWQuestSingletone DefaultInstance;
	return DefaultInstance;
}