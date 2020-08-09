// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_GameplayAbility.h"

UMBR_GameplayAbility::UMBR_GameplayAbility()
{
	// Init Properties
	AbilityInputID = EMBR_AbilityInputID::None;
	AbilityID = EMBR_AbilityInputID::None;

	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
