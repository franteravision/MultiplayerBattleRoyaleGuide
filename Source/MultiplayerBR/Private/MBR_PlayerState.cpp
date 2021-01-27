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

	KillCount = 0;
	DeadTag = FGameplayTag::RequestGameplayTag(FName("MBR.State.Dead"));
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
	if (!IsAlive() && IsValid(AbilitySystemComponent) && GetLocalRole() == ROLE_Authority && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		AMultiplayerBRCharacter* characterRef = Cast<AMultiplayerBRCharacter>(GetPawn());
		if (IsValid(characterRef))
		{
			AActor* killerActor = Data.GEModData->EffectSpec.GetEffectContext().GetEffectCauser();
			if(IsValid(killerActor))
			{
				AMultiplayerBRCharacter* KillerCharacter = Cast<AMultiplayerBRCharacter>(killerActor);
				if (IsValid(KillerCharacter))
				{
					characterRef->Server_Die(KillerCharacter);
				}
			}
		}
	}
}

//=====================================================================================================================
void AMBR_PlayerState::ScoreKill()
{
	KillCount++;
}
