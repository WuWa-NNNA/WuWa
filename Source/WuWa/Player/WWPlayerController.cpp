#include "Player/WWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/UWorldUserWidget.h"
#include "Character/Resonator/Resonator.h"
#include "Stat/Player/PlayerStatComponent.h"
#include "Stat/Player/LunoStatComponent.h"

#include "Character/Monster/Sigillum/Sigillum.h"
#include "Stat/Monster/SigillumStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"

AWWPlayerController::AWWPlayerController()
{
	CurrentInputType = EInputType::Game;

	static ConstructorHelpers::FClassFinder<UUWorldUserWidget> HUDWidgetAsset(TEXT("/Game/PCH/UI/Blueprint/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetAsset.Succeeded())
	{
		HUDWidgetClass = HUDWidgetAsset.Class;
	}
}

void AWWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(Concerto1Action, ETriggerEvent::Started, this, &AWWPlayerController::Concerto1);
		EnhancedInputComponent->BindAction(Concerto2Action, ETriggerEvent::Started, this, &AWWPlayerController::Concerto2);
	}
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	SetInputMappingContext(CurrentInputType);

	SpawnResonators();
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
			PlayerStat->OnHpChagned.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateHpBar);
			PlayerStat->FOnSkillEStart.AddUObject(MainHUDWidget, &UUWorldUserWidget::SkillCoolEActive);
			PlayerStat->OnBaseSkillchange.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateSkillIcon);
			PlayerStat->OnRGaugaChanged.AddUObject(MainHUDWidget, &UUWorldUserWidget::UpdateRGauge);
			PlayerStat->OnRSart.AddUObject(MainHUDWidget, &UUWorldUserWidget::HUDHidden);
			PlayerStat->OnREnd.AddUObject(MainHUDWidget, &UUWorldUserWidget::HUDVisible);
			PlayerStat->OnLockOn.AddUObject(MainHUDWidget, &UUWorldUserWidget::TriggerTouchLockOnAnimation);
			PlayerStat->OnPlayIconAnimation.AddUObject(MainHUDWidget, &UUWorldUserWidget::TriggerTouchBaseAttackAnimation);
			PlayerStat->OnPlayIcon4Animation.AddUObject(MainHUDWidget, &UUWorldUserWidget::TriggerTouchBurstAnimation);
			
			MainHUDWidget->SetMaxHp(PlayerStat->GetMaxHP());
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

void AWWPlayerController::OnUnPossess()
{
	if (AResonator* OldResonator = Cast<AResonator>(GetPawn()))
	{
		if (UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(OldResonator->GetStatComponent()))
		{
			PlayerStat->OnHpChagned.RemoveAll(MainHUDWidget);
			PlayerStat->FOnSkillEStart.RemoveAll(MainHUDWidget);
			PlayerStat->OnBaseSkillchange.RemoveAll(MainHUDWidget);
			PlayerStat->OnRGaugaChanged.RemoveAll(MainHUDWidget);
			PlayerStat->OnRSart.RemoveAll(MainHUDWidget);
			PlayerStat->OnREnd.RemoveAll(MainHUDWidget);
			PlayerStat->OnLockOn.RemoveAll(MainHUDWidget);
			PlayerStat->OnPlayIconAnimation.RemoveAll(MainHUDWidget);
			PlayerStat->OnPlayIcon4Animation.RemoveAll(MainHUDWidget);
		}

		if (ULunoStatComponent* LunoStat =
			Cast<ULunoStatComponent>(OldResonator->GetStatComponent()))
		{
			LunoStat->OnChangeCrescentTime.RemoveAll(MainHUDWidget);
			LunoStat->OnBaseEndCrescentTime.RemoveAll(MainHUDWidget);
		}
	}

	Super::OnUnPossess();
}

void AWWPlayerController::Concerto1()
{
	ProcessConcerto(0);
}

void AWWPlayerController::Concerto2()
{
	ProcessConcerto(1);
}

void AWWPlayerController::ProcessConcerto(int NextResonatorIndex)
{
	if (!PartyResonators.IsEmpty() && PartyResonators[NextResonatorIndex])
	{
		if (CurrentResonatorIndex == NextResonatorIndex)
		{
			return;
		}
		
		AResonator* CurrentResonator = PartyResonators[CurrentResonatorIndex];
		AResonator* NextResonator = PartyResonators[NextResonatorIndex];
		if (!CurrentResonator->CanConcerto())
		{
			return;
		}

		CurrentResonator->ConcertoOut();
		NextResonator->ConcertoIn(CurrentResonator);

		NextResonator->SetCameraLag(false);
		FRotator SavedRotator = GetControlRotation();

		Possess(NextResonator);

		SetControlRotation(SavedRotator);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[NextResonator]()
			{
				if (IsValid(NextResonator))
				{
					NextResonator->SetCameraLag(true);
				}
			},
			0.05f,
			false
		);

		CurrentResonatorIndex = NextResonatorIndex;
	}
}

void AWWPlayerController::SpawnResonators()
{
	for (TSubclassOf<AResonator> ResonatorClass : PartyResonatorClasses)
	{
		if (ResonatorClass)
		{
			AResonator* Resonator = GetWorld()->SpawnActor<AResonator>(ResonatorClass, ResonatorSpawnTransform);
			PartyResonators.Add(Resonator);
			Resonator->SetActorHiddenInGame(true);
			Resonator->SetActorEnableCollision(false);
		}
	}

	AResonator* PossessResonator = (!PartyResonators.IsEmpty() ? PartyResonators[0] : nullptr);
	if (PossessResonator)
	{
		Possess(PossessResonator);
		PossessResonator->SetActorHiddenInGame(false);
		PossessResonator->SetActorEnableCollision(true);
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
			return;
		}
		MainHUDWidget->InitializeBossUISetting(BossStat);
	}
}
