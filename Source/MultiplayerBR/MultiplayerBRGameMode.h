// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerBRGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerBRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerBRGameMode();

	void CheckForPlayersAlive();

	void PlayerKilled(AController* KilllerController, AController* VictimController);
};



