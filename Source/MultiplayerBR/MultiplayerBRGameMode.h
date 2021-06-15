// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerBRGameMode.generated.h"

class APlayerStart;
class AMBR_Collectable;
class AMBR_Spawner;

UCLASS(minimalapi)
class AMultiplayerBRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerBRGameMode();

	virtual void BeginPlay() override;

	void PlayerKilled(AController* KilllerController, AController* VictimController);

	void InitPlayerStarts();

	/** select best spawn point for player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	TArray<APlayerStart*> WorldPlayerStarts;

	void SetupWorldSpawners();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<AMBR_Collectable>> SpawnableCollectables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMBR_Spawner> SpawnerSubclass;
};



