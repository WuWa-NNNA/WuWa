#pragma once

#include "CoreMinimal.h"
#include "EDistanceRange.generated.h"

UENUM(BlueprintType)
enum class EDistanceRange : uint8
{
    Melee   UMETA(DisplayName = "초단거리"),
    Short   UMETA(DisplayName = "단거리"),
    Mid     UMETA(DisplayName = "중거리"),
    Long    UMETA(DisplayName = "원거리")
};