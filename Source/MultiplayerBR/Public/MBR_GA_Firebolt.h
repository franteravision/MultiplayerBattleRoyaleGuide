// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MBR_GameplayAbility.h"
#include "MBR_GA_Firebolt.generated.h"

class UAnimMontage;
class AMBR_Projectile;

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API UMBR_GA_Firebolt : public UMBR_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* CastMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMBR_Projectile> FireboltClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag ProjectileSpawnTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag StopMontageTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileRange;

protected:
	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
