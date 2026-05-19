#pragma once

#include "CoreMinimal.h"
#include "EDistanceRange.generated.h"

UENUM(BlueprintType)
enum class EDistanceRange : uint8
{
    Short   UMETA(DisplayName = "단거리"),
    Long    UMETA(DisplayName = "원거리")
};