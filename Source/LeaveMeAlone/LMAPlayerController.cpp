// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMAPlayerController.h"

ALMAPlayerController::ALMAPlayerController() {}

void ALMAPlayerController::BeginPlay() 
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ALMAPlayerController::BeginSpectatingState() 
{
	SetControlRotation(FRotator(-50.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}
