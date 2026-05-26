#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WWQuestData.generated.h"

USTRUCT(BlueprintType)
struct FWWQuestData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FWWQuestData() : QuestID(0), QuestName(TEXT("")), QuestType(TEXT("")), QuestDescription(TEXT("")), QuestDetailedDescription(TEXT("")) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	int32 QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	FString QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	FString QuestDetailedDescription;

};