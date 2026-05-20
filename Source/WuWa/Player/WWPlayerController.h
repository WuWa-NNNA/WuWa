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

protected:
	virtual void OnPossess(APawn* InPawn) override;

public :
	void CreateHUDWidget();

	UFUNCTION(BlueprintCallable)
	void SpawnBoss();

private:
	UPROPERTY(VisibleAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	EInputType CurrentInputType;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TMap<EInputType, TObjectPtr<class UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUWorldUserWidget> MainHUDWidget;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUWorldUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> DashGaugeWidget;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> DashWidgetClass;


};
