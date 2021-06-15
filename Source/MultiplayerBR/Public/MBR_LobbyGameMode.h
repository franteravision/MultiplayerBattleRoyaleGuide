// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerBR/MultiplayerBRGameMode.h"
#include "MBR_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API AMBR_LobbyGameMode : public AMultiplayerBRGameMode
{
	GENERATED_BODY()

	AMBR_LobbyGameMode();
	
public:
	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	/** TODO */
	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	int MaxPlayersToTravel;

	/** TODO */
	int CurrentPlayersOnLobby;
};
