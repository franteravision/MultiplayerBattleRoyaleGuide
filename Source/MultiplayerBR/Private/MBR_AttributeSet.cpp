// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

//=====================================================================================================================
UMBR_AttributeSet::UMBR_AttributeSet()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
	HealthRegen = 0.5f;

	MaxMana = 100.f;
	Mana = MaxMana;
	ManaRegen = 1.f;
}

//=====================================================================================================================
void UMBR_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

//=====================================================================================================================
void UMBR_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Checks if Health attribute was affected
	if (
		Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<UProperty>(UMBR_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMBR_AttributeSet, Health))
		)
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
		OnHealthChanged.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
	// Checks if Mana attribute was affected
	else if (
		Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<UProperty>(UMBR_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMBR_AttributeSet, Mana))
		)
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManaChanged.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}
}

//=====================================================================================================================
void UMBR_AttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
