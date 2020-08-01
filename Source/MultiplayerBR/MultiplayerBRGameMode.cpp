// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerBRGameMode.h"
#include "MultiplayerBRCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerBRGameMode::AMultiplayerBRGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
