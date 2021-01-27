// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MBR_PlayerState.generated.h"

class UAbilitySystemComponent;
class UMBR_AttributeSet;

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API AMBR_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// Constructor
	AMBR_PlayerState();

	// ASC Getter
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// AttributeSet Getter
	UMBR_AttributeSet* GetAttributeSet() const;

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UMBR_AttributeSet* AttributeSet;

	// Checks if Character is currently alive
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	bool IsAlive() const;

	/** Attribute Getters */
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	float GetHealthRegenRate() const;

	virtual void BeginPlay() override;

	FDelegateHandle HealthChangedDelegateHandle;

	virtual void  OnHealthChanged(const FOnAttributeChangeData& Data);

	void ScoreKill();

	int KillCount;

	FGameplayTag DeadTag;
};
