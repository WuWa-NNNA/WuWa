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
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

private:
	void SetInputMappingContext(EInputType InNewInputType);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void Concerto1();
	void Concerto2();

private:
	void ProcessConcerto(int NextResonatorIndex);

private:
	void SpawnResonators();
	void CreateHUDWidget();

public:
	UFUNCTION(BlueprintCallable, Category = "Staging", meta = (AllowPrivateAccess = "true"))
	void SpawnBoss();

private:
	UPROPERTY(VisibleAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	EInputType CurrentInputType;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TMap<EInputType, TObjectPtr<class UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Concerto1Action;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Concerto2Action;

private:
	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUWorldUserWidget> MainHUDWidget;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUWorldUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> DashGaugeWidget;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> DashWidgetClass;

private:
	UPROPERTY(EditAnywhere, Category = "Party", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AResonator>> PartyResonatorClasses;

	UPROPERTY()
	TArray<TObjectPtr<class AResonator>> PartyResonators;

	uint32 CurrentResonatorIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party", meta = (AllowPrivateAccess = "true"))
	FTransform ResonatorSpawnTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party", meta = (AllowPrivateAccess = "true"))
	FTransform ResonatorSpawnTransform2;

};
