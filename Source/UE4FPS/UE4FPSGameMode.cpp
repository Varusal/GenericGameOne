// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4FPSGameMode.h"
#include "UE4FPSHUD.h"
#include "UE4FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4FPSGameMode::AUE4FPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AUE4FPSHUD::StaticClass();
}
