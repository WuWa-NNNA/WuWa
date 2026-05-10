#include "Player/WWPlayerController.h"
#include "EnhancedInputSubsystems.h"

AWWPlayerController::AWWPlayerController()
{
	CurrentInputType = EInputType::Game;
}

void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMappingContext(CurrentInputType);
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
