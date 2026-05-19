#include "Player/WWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/UWorldUserWidget.h"
#include "Character/Resonator/Resonator.h"
#include "Stat/Player/PlayerStatComponent.h"


AWWPlayerController::AWWPlayerController()
{
	CurrentInputType = EInputType::Game;

	static ConstructorHelpers::FClassFinder<UUWorldUserWidget> HUDWidgetAsset(TEXT("/Game/PCH/UI/Blueprint/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetAsset.Succeeded())
	{
		HUDWidgetClass = HUDWidgetAsset.Class;
	}
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	SetInputMappingContext(CurrentInputType);

	CreateHUDWidget();
}

void AWWPlayerController::SetInputMappingContext(EInputType InNewInputType)
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSystem)
	{
		InputSystem->ClearAllMappings();
		InputSystem->AddMappingContext(InputMappingContexts[InNewInputType], 0);
	}
}

void AWWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Log, TEXT("Succed UPlayerStatComponent1"));
	if (!MainHUDWidget)
	{
		CreateHUDWidget();
		UE_LOG(LogTemp, Log, TEXT("NoCreate"));
	}
	AResonator* Resonator = Cast<AResonator>(InPawn);
	if (Resonator && MainHUDWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Succed UPlayerStatComponent2"));

		UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Resonator->GetStatComponent());
		if (PlayerStat)
		{
			PlayerStat->OnHpChagned.Clear();

			PlayerStat->OnHpChagned.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateHpBar);
			PlayerStat->OnDashChanged.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateMainHUD);
			PlayerStat->FOnSkillEStart.AddUObject(MainHUDWidget, &UUWorldUserWidget::SkillCoolEActive);
			PlayerStat->FOnSkillRStart.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateSkillCoolR);
			PlayerStat->OnBaseSkillchange.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateSkillIcon);
			PlayerStat->OnRGaugaChanged.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateRGauge);
			UE_LOG(LogTemp, Log, TEXT("Succed UPlayerStatComponent3"));

			MainHUDWidget->SetMaxHp(PlayerStat->GetMaxHP());
			MainHUDWidget->SetMaxDash(PlayerStat->GetMaxDash());
			MainHUDWidget->UpdateHpBar(PlayerStat->GetCurrentHP());
			MainHUDWidget->UpdateRGauge(PlayerStat->GetRGauge());
		}
	}
}

void AWWPlayerController::CreateHUDWidget()
{
	if (MainHUDWidget)
	{
		return;
	}
	MainHUDWidget = CreateWidget<UUWorldUserWidget>(this, HUDWidgetClass);

	if (MainHUDWidget)
	{
		MainHUDWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("Succed MainHUDWidget"));

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed MainHUDWidget"));

	}
}
