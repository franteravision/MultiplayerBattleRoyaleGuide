// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerBRCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "MBR_AttributeSet.h"
#include "MBR_GameplayAbility.h"
#include "MBR_PlayerState.h"
#include "MBR_PlayerController.h"
#include "MultiplayerBRGameMode.h"

//////////////////////////////////////////////////////////////////////////
// AMultiplayerBRCharacter

AMultiplayerBRCharacter::AMultiplayerBRCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	IsInputBound = false;
	AIControllerClass = AMBR_PlayerController::StaticClass();
	IsDying = false;
}

//=====================================================================================================================
void AMultiplayerBRCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMBR_PlayerState* playerState = GetPlayerState<AMBR_PlayerState>();
	if (IsValid(playerState))
	{
		AbilitySystemComponent = playerState->GetAbilitySystemComponent();
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
		AttributeSet = playerState->GetAttributeSet();
		AttributeSet->OnHealthChanged.AddDynamic(this, &AMultiplayerBRCharacter::HealthPercentageChanged);
		AttributeSet->OnManaChanged.AddDynamic(this, &AMultiplayerBRCharacter::ManaPercentageChanged);
		SetupAbilities();
		SetupEffects();
	}
}

//=====================================================================================================================
void AMultiplayerBRCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMultiplayerBRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiplayerBRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiplayerBRCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMultiplayerBRCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMultiplayerBRCharacter::LookUpAtRate);

	// set up ASC input bindings -- also done on OnRep_PlayerState for races
	SetupGASInputs();
}

//=====================================================================================================================
UAbilitySystemComponent* AMultiplayerBRCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//=====================================================================================================================
void AMultiplayerBRCharacter::SetupGASInputs()
{
	if (!IsInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(
			InputComponent,
			FGameplayAbilityInputBinds(
				FString("Confirm"),
				FString("Cancel"),
				FString("EMBR_AbilityInputID"),
				static_cast<int32>(EMBR_AbilityInputID::Confirm),
				static_cast<int32>(EMBR_AbilityInputID::Cancel)
			));

		IsInputBound = true;
	}
}

//=====================================================================================================================
void AMultiplayerBRCharacter::SetupAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilititesGiven)
	{
		return;
	}

	for (TSubclassOf<UMBR_GameplayAbility>& currentAbility : StartingAbilities)
	{
		if (IsValid(currentAbility))
		{
			UMBR_GameplayAbility* defaultObj = currentAbility->GetDefaultObject<UMBR_GameplayAbility>();
			FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(defaultObj, 1, static_cast<int32>(defaultObj->AbilityInputID), this);
			AbilitySystemComponent->GiveAbility(
				abilitySpec
			);
		}
	}

	AbilititesGiven = true;
}

//=====================================================================================================================
void AMultiplayerBRCharacter::SetupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || EffectsGiven)
	{
		return;
	}

	FGameplayEffectContextHandle effectContext = AbilitySystemComponent->MakeEffectContext();
	effectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartingEffects)
	{
		FGameplayEffectSpecHandle newHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, effectContext);
		if (newHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(), GetAbilitySystemComponent());
		}
	}

	EffectsGiven = true;
}

//=====================================================================================================================
void AMultiplayerBRCharacter::Server_Die_Implementation(AMultiplayerBRCharacter* Killer)
{
	if (IsDying)
	{
		return;
	}

	IsDying = true;

	if (IsValid(DeadEffectClass))
	{
		FGameplayEffectContextHandle EffectContext;
		AbilitySystemComponent->ApplyGameplayEffectToSelf(DeadEffectClass->GetDefaultObject<UGameplayEffect>(), 1.0f, EffectContext);
	}

	AMBR_PlayerState* killerPlayerState = Cast<AMBR_PlayerState>(Killer->GetPlayerState());
	if (IsValid(killerPlayerState))
	{
		killerPlayerState->ScoreKill();
	}

	AMultiplayerBRGameMode* GameMode = Cast<AMultiplayerBRGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->PlayerKilled(Killer->GetController(), GetController());

	Multicast_OnDeath();
}

//=====================================================================================================================
void AMultiplayerBRCharacter::Multicast_OnDeath_Implementation()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		Destroy();
	}
}

//=====================================================================================================================
void AMultiplayerBRCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMBR_PlayerState* playerState = GetPlayerState<AMBR_PlayerState>();
	if (IsValid(playerState))
	{
		AbilitySystemComponent = playerState->GetAbilitySystemComponent();
		
		GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
		
		AttributeSet = playerState->GetAttributeSet();
		AttributeSet->OnHealthChanged.AddDynamic(this, &AMultiplayerBRCharacter::HealthPercentageChanged);
		AttributeSet->OnManaChanged.AddDynamic(this, &AMultiplayerBRCharacter::ManaPercentageChanged);
		SetupGASInputs();
	}
}

//=====================================================================================================================
void AMultiplayerBRCharacter::HealthPercentageChanged(float Health, float MaxHealth)
{
	Client_HealthPercentageChanged(Health, MaxHealth);
}

//=====================================================================================================================
void AMultiplayerBRCharacter::Client_HealthPercentageChanged_Implementation(float Health, float MaxHealth)
{
	OnHealthPercentChangedDelegate.Broadcast(Health/MaxHealth);
}

//=====================================================================================================================
void AMultiplayerBRCharacter::ManaPercentageChanged(float Mana, float MaxMana)
{
	Client_ManaPercentageChanged(Mana, MaxMana);
}

//=====================================================================================================================
void AMultiplayerBRCharacter::Client_ManaPercentageChanged_Implementation(float Mana, float MaxMana)
{
	OnManaPercentChangedDelegate.Broadcast(Mana/MaxMana);
}

//=====================================================================================================================
void AMultiplayerBRCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

//=====================================================================================================================
void AMultiplayerBRCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

//=====================================================================================================================
void AMultiplayerBRCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

//=====================================================================================================================
void AMultiplayerBRCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//=====================================================================================================================
void AMultiplayerBRCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//=====================================================================================================================
void AMultiplayerBRCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//=====================================================================================================================
void AMultiplayerBRCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
