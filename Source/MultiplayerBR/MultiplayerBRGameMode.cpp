// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerBRGameMode.h"
#include "MultiplayerBRCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MBR_PlayerState.h"
#include "MBR_GameState.h"
#include "MBR_PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "MBR_Spawner.h"
#include "MBR_Collectable.h"

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
void AMultiplayerBRGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerStarts();
	SetupWorldSpawners();
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

//=====================================================================================================================
void AMultiplayerBRGameMode::InitPlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* CurrentPlayerStart = *It;
		if (IsValid(CurrentPlayerStart))
		{
			WorldPlayerStarts.Add(CurrentPlayerStart);
		}
	}
}

//=====================================================================================================================
void AMultiplayerBRGameMode::SetupWorldSpawners()
{
	int CollectableIndex = 0;
	for (TActorIterator<AMBR_Spawner> It(GetWorld()); It; ++It)
	{
		AMBR_Spawner* CurrentSpawner = *It;
		CurrentSpawner->SetCollectableSubclass(SpawnableCollectables[CollectableIndex % SpawnableCollectables.Num()]);
		CurrentSpawner->RespawnCollectable();
		CollectableIndex++;
	}
}

//=====================================================================================================================
AActor* AMultiplayerBRGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* BestStart = nullptr;
	int32 RandomIndex = FMath::RandHelper(WorldPlayerStarts.Num() - 1);
	if(RandomIndex >= 0)
	{
		BestStart = WorldPlayerStarts[RandomIndex];
		WorldPlayerStarts.RemoveAt(RandomIndex);
	}
	
	if(IsValid(BestStart))
	{
		return BestStart;
	}
	else
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}
}
