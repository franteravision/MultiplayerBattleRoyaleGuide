// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MBR_AttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChangedDelegate, float, Mana, float, MaxMana);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
  GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API UMBR_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMBR_AttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void AdjustAttributeForMaxChange(
		FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty
	);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMBR_AttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMBR_AttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "HealthRegen")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UMBR_AttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMBR_AttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "MaxMana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMBR_AttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "ManaRegen")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UMBR_AttributeSet, ManaRegen);

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	FOnHealthChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Mana")
	FOnManaChangedDelegate OnManaChanged;
};
