#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LunoArrow.generated.h"

UCLASS()
class WUWA_API ALunoArrow : public AActor
{
	GENERATED_BODY()
	
public:
	ALunoArrow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
