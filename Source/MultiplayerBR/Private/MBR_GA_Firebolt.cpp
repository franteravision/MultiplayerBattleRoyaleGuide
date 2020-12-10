// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_GA_Firebolt.h"
#include "MBRT_PlayMontageAndWaitForEvent.h"
#include "MBR_Projectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "MultiplayerBR/MultiplayerBRCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

//=====================================================================================================================
void UMBR_GA_Firebolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UMBRT_PlayMontageAndWaitForEvent* montageTask = UMBRT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this,
		NAME_None,
		CastMontage,
		FGameplayTagContainer(),
		1.f,
		NAME_None,
		false,
		1.f);

	montageTask->OnBlendOut.AddDynamic(this, &UMBR_GA_Firebolt::OnMontageCompleted);
	montageTask->OnCompleted.AddDynamic(this, &UMBR_GA_Firebolt::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UMBR_GA_Firebolt::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UMBR_GA_Firebolt::OnMontageCancelled);
	montageTask->EventReceived.AddDynamic(this, &UMBR_GA_Firebolt::EventReceived);

	montageTask->ReadyForActivation();
}

//=====================================================================================================================
void UMBR_GA_Firebolt::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//=====================================================================================================================
void UMBR_GA_Firebolt::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//=====================================================================================================================
void UMBR_GA_Firebolt::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == StopMontageTag)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
	{
		AMultiplayerBRCharacter* character = Cast<AMultiplayerBRCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FVector startLoc = character->GetMesh()->GetSocketLocation(FName("abilityRightSocket"));
		FVector endLoc = character->GetCameraBoom()->GetComponentLocation() + (character->GetFollowCamera()->GetForwardVector() * ProjectileRange);
		FRotator projectileRotation = UKismetMathLibrary::FindLookAtRotation(startLoc, endLoc);

		FTransform spawnTransform = character->GetMesh()->GetSocketTransform(FName("abilityRightSocket"));
		spawnTransform.SetRotation(projectileRotation.Quaternion());
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMBR_Projectile* firebolt = GetWorld()->SpawnActorDeferred<AMBR_Projectile>(FireboltClass, spawnTransform, GetOwningActorFromActorInfo(), character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		firebolt->Multicast_IgnoreActor(character);
		firebolt->Range = ProjectileRange;
		firebolt->FinishSpawning(spawnTransform);
	}
}
