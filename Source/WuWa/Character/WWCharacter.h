#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Stat/WWStatComponent.h"
#include "WWCharacter.generated.h"

UCLASS()
class WUWA_API AWWCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWWCharacter(const FObjectInitializer& ObjectInitializer);

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UWWStatComponent> Stat;
};
