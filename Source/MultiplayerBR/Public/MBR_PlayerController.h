// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MBR_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API AMBR_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;
};
