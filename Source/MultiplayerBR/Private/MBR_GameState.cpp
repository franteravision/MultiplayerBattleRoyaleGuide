// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_GameState.h"
#include "MBR_PlayerController.h"
#include "MBR_PlayerState.h"

//=====================================================================================================================
void AMBR_GameState::CheckWinCondition()
{
	int AlivePlayerCounter = 0;
	AMBR_PlayerController* WinningPlayerController = nullptr;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMBR_PlayerController* PlayerController = Cast<AMBR_PlayerController>(It->Get());
		if (IsValid(PlayerController))
		{
			AMBR_PlayerState* PlayerState = PlayerController->GetPlayerState<AMBR_PlayerState>();
			if (IsValid(PlayerState))
			{
				if (PlayerState->IsAlive())
				{
					AlivePlayerCounter++;

					if (AlivePlayerCounter > 1)
					{
						return;
					}
					else
					{
						WinningPlayerController = PlayerController;
					}
				}
			}
		}
	}

	WinningPlayerController->GameConclussion(true);
}
