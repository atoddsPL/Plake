// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlakeGameMode.h"
#include "PlakeCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlakeGameMode::APlakeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
