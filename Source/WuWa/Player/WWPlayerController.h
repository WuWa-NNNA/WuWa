#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WWPlayerController.generated.h"

UENUM()
enum class EInputType : uint8
{
	Game,
	UI
};

UCLASS()
class WUWA_API AWWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWWPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	void SetInputMappingContext(EInputType InNewInputType);

private:
	UPROPERTY(VisibleAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	EInputType CurrentInputType;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TMap<EInputType, TObjectPtr<class UInputMappingContext>> InputMappingContexts;
};
