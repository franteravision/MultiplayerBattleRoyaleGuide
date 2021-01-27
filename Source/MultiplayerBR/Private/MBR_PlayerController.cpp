// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_PlayerController.h"
#include "MBR_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

//=====================================================================================================================
void AMBR_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetViewTarget(InPawn);

	AMBR_PlayerState* playerState = GetPlayerState<AMBR_PlayerState>();
	if (playerState)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, InPawn);
	}
}

//=====================================================================================================================
void AMBR_PlayerController::GameConclussion(bool WasSuccesful)
{
	bool TempBool = WasSuccesful;
	BP_GameConclussion(WasSuccesful);

	TArray<AActor*> SpectatingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpectatingActorClass, SpectatingActors);

	SetViewTargetWithBlend(SpectatingActors[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
}
