#include "Player/WWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/UWorldUserWidget.h"
#include "Character/Resonator/Resonator.h"
#include "Stat/Player/PlayerStatComponent.h"
#include "Stat/Player/LunoStatComponent.h"

#include "Character/Monster/Sigillum/Sigillum.h"
#include "Stat/Monster/SigillumStatComponent.h"
#include "Kismet/GameplayStatics.h"

AWWPlayerController::AWWPlayerController()
{
	CurrentInputType = EInputType::Game;

	static ConstructorHelpers::FClassFinder<UUWorldUserWidget> HUDWidgetAsset(TEXT("/Game/PCH/UI/Blueprint/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetAsset.Succeeded())
	{
		HUDWidgetClass = HUDWidgetAsset.Class;
	}

	/*static ConstructorHelpers::FClassFinder<UUserWidget> DashWidgetAsset(TEXT("/Game/PCH/UI/Blueprint/WBP_Gauage.WBP_Gauage"));
	if (DashWidgetAsset.Succeeded())
	{
		DashWidgetClass = DashWidgetAsset.Class;
	}*/
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
	if (!MainHUDWidget)
	{
		CreateHUDWidget();
	}
	AResonator* Resonator = Cast<AResonator>(InPawn);
	if (Resonator && MainHUDWidget)
	{
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
			PlayerStat->OnRSart.AddUObject(MainHUDWidget, &UUWorldUserWidget::HUDHidden);
			PlayerStat->OnREnd.AddUObject(MainHUDWidget, &UUWorldUserWidget::HUDVisible);

			MainHUDWidget->SetMaxHp(PlayerStat->GetMaxHP());
			MainHUDWidget->SetMaxDash(PlayerStat->GetMaxDash());
			MainHUDWidget->UpdateHpBar(PlayerStat->GetCurrentHP());
			MainHUDWidget->UpdateRGauge(PlayerStat->GetRGauge());
		}
		ULunoStatComponent* LunoStat = Cast<ULunoStatComponent>(Resonator->GetStatComponent());
		if (LunoStat)
		{
			LunoStat->OnChangeCrescentTime.AddUObject(MainHUDWidget, &UUWorldUserWidget::ChangedTransformationGauge);
			LunoStat->OnBaseEndCrescentTime.AddUObject(MainHUDWidget, &UUWorldUserWidget::SkillCoolEDisable);
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
	}
}

void AWWPlayerController::SpawnBoss()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASigillum::StaticClass());
	ASigillum* Boss = Cast<ASigillum>(FoundActor);
	if (!Boss)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Finde Sigllium"));
		return;
	}
	if (MainHUDWidget)
	{
		USigillumStatComponent* BossStat = Cast<USigillumStatComponent>(Boss->GetStatComponent());
		if (!BossStat)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Finde USigillumStatComponent"));
			return;
		}
		MainHUDWidget->InitializeBossUISetting(BossStat);
	}

}
