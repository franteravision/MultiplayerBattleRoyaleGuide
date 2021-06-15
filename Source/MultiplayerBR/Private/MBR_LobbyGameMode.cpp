// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_LobbyGameMode.h"

//=====================================================================================================================
AMBR_LobbyGameMode::AMBR_LobbyGameMode()
{
	CurrentPlayersOnLobby = 0;
	MaxPlayersToTravel = 3;
}

//=====================================================================================================================
void AMBR_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (IsValid(NewPlayer))
	{
		CurrentPlayersOnLobby++;

		if (CurrentPlayersOnLobby >= MaxPlayersToTravel)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;
				World->ServerTravel("/Game/ThirdPersonCPP/Maps/BattleRoyaleMap?listen");
			}
		}
	}
}
