// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MBR_GameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API AMBR_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void CheckWinCondition();
};
