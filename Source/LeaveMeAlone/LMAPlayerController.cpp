// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMAPlayerController.h"

ALMAPlayerController::ALMAPlayerController() {}

void ALMAPlayerController::BeginPlay() 
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
