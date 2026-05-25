#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "WWSkillIconData.generated.h"

USTRUCT(BlueprintType)
struct FWWSkillIconData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FWWSkillIconData()
		: SkillID(0)
		, Cooldown(0.f)
	{
	}

public:

	// 스킬 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillID;

	// 스킬 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;

	// 스킬 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> SkillIcon;

	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

};