// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerBRGameMode.h"
#include "MultiplayerBRCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MBR_PlayerState.h"
#include "MBR_GameState.h"
#include "MBR_PlayerController.h"

AMultiplayerBRGameMode::AMultiplayerBRGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

//=====================================================================================================================
void AMultiplayerBRGameMode::PlayerKilled(AController* KilllerController, AController* VictimController)
{
	AMBR_PlayerState* KillerPlayerState = IsValid(KilllerController) ? KilllerController->GetPlayerState<AMBR_PlayerState>() : NULL;
	AMBR_PlayerState* VictimPlayerState = IsValid(VictimController) ? VictimController->GetPlayerState<AMBR_PlayerState>() : NULL;

	if (IsValid(VictimPlayerState))
	{
		AMBR_PlayerController* MBRVictimController = Cast<AMBR_PlayerController>(VictimController);
		if (IsValid(VictimController))
		{
			MBRVictimController->GameConclussion(false);
		}
	}

	AMBR_GameState* WorldGameState = GetWorld()->GetGameState<AMBR_GameState>();
	if (IsValid(WorldGameState))
	{
		WorldGameState->CheckWinCondition();
	}

}
