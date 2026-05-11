#include "Game/WWGameMode.h"
#include "Player/WWPlayerController.h"

AWWGameMode::AWWGameMode()
{
	PlayerControllerClass = AWWPlayerController::StaticClass();
}
