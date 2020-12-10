// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "MBR_AttributeSet.h"
#include "MultiplayerBR/MultiplayerBRCharacter.h"
#include "GameplayEffectExtension.h"
#include "Kismet/KismetSystemLibrary.h"

//=====================================================================================================================
AMBR_PlayerState::AMBR_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UMBR_AttributeSet>(TEXT("Attribute Set"));

	NetUpdateFrequency = 100.f;
}

//=====================================================================================================================
UAbilitySystemComponent* AMBR_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//=====================================================================================================================
UMBR_AttributeSet* AMBR_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

//=====================================================================================================================
bool AMBR_PlayerState::IsAlive() const
{
	return GetHealth() > 0.f;
}

//=====================================================================================================================
float AMBR_PlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

//=====================================================================================================================
float AMBR_PlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

//=====================================================================================================================
float AMBR_PlayerState::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegen();
}

//=====================================================================================================================
void AMBR_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		// Attribute Change Callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AMBR_PlayerState::OnHealthChanged);
	}
}

//=====================================================================================================================
void AMBR_PlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	//AActor* killerActor = Data.GEModData->EffectSpec.GetEffectContext().GetEffectCauser();
	//UE_LOG(LogTemp, Warning, TEXT("AMBR_PlayerState::OnHealthChanged: %s"), *UKismetSystemLibrary::GetDisplayName(killerActor));
	if (!IsAlive() && IsValid(AbilitySystemComponent))
	{
		AMultiplayerBRCharacter* characterRef = Cast<AMultiplayerBRCharacter>(GetPawn());
		if (IsValid(characterRef))
		{
			characterRef->Die();
		}
	}
}
