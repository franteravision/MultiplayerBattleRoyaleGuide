// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>

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
		FindFieldChecked<FProperty>(UMBR_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMBR_AttributeSet, Health))
		)
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
		OnHealthChanged.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

		//UE_LOG(LogTemp, Warning, TEXT("%s Health: %f"), *GetOwningActor()->GetName(), Health.GetCurrentValue());
	}
	// Checks if Mana attribute was affected
	else if (
		Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UMBR_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMBR_AttributeSet, Mana))
		)
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManaChanged.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
		//UE_LOG(LogTemp, Warning, TEXT("%s Mana: %f"), *GetOwningActor()->GetName(), Mana.GetCurrentValue());
	}
}

//=====================================================================================================================
void UMBR_AttributeSet::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMBR_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMBR_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMBR_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMBR_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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

//=====================================================================================================================
void UMBR_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMBR_AttributeSet, Health, OldHealth);
}

//=====================================================================================================================
void UMBR_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMBR_AttributeSet, MaxHealth, OldMaxHealth);
}

//=====================================================================================================================
void UMBR_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMBR_AttributeSet, Mana, OldMana);
}

//=====================================================================================================================
void UMBR_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMBR_AttributeSet, MaxMana, OldMaxMana);
}

//=====================================================================================================================
void UMBR_AttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMBR_AttributeSet, HealthRegen, OldHealthRegen);
}

//=====================================================================================================================
void UMBR_AttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMBR_AttributeSet, ManaRegen, OldManaRegen);
}
