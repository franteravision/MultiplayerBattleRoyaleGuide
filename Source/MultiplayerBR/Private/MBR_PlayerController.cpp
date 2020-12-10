// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_PlayerController.h"
#include "MBR_PlayerState.h"
#include "AbilitySystemComponent.h"

void AMBR_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMBR_PlayerState* playerState = GetPlayerState<AMBR_PlayerState>();
	if (playerState)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, InPawn);
	}
}
