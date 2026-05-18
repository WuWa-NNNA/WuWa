#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatTeamInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCombatTeamInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Resonator,
	Monster,
	Neutral
};

class WUWA_API ICombatTeamInterface
{
	GENERATED_BODY()

public:
	virtual ETeamType GetTeamType() const = 0;
};
