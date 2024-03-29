// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MultiplayerBRCharacter.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UMBR_AttributeSet;
class UMBR_GameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourcePercentChanged, float, Percent);

UCLASS(config=Game)
class AMultiplayerBRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	virtual void BeginPlay() override;
public:
	AMultiplayerBRCharacter();

	UPROPERTY(BlueprintAssignable)
	FOnResourcePercentChanged OnHealthPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnResourcePercentChanged OnManaPercentChangedDelegate;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	virtual void PossessedBy(AController* NewController) override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool IsInputBound;

	/* --- Gameplay Ability System Start --- */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	UMBR_AttributeSet* AttributeSet;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, Category = "Gameplay Abilities")
	TArray<TSubclassOf<UMBR_GameplayAbility>> StartingAbilities;

	UPROPERTY(EditAnywhere, Category = "Gameplay Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartingEffects;

	void SetupGASInputs();

	void SetupAbilities();

	bool AbilititesGiven;

	void SetupEffects();

	bool EffectsGiven;

	/* --- Gameplay Ability System End --- */

	/* --- Win Condition Start --- */

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DeadEffectClass;

	bool IsDying;

	UFUNCTION(Server, Reliable)
	virtual void Server_Die(AMultiplayerBRCharacter* Killer);

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_OnDeath();

	virtual void OnRep_PlayerState() override;

	UFUNCTION()
	void HealthPercentageChanged(float Health, float MaxHealth);

	UFUNCTION(Client, Reliable)
	void Client_HealthPercentageChanged(float Health, float MaxHealth);

	UFUNCTION()
	void ManaPercentageChanged(float Mana, float MaxMana);

	UFUNCTION(Client, Reliable)
	void Client_ManaPercentageChanged(float Mana, float MaxMana);

	/* --- Win Condition Start --- */
};
