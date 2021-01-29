// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBR_Collectable.generated.h"

class UCapsuleComponent;
class UParticleSystemComponent;
class UGameplayEffect;

UCLASS()
class MULTIPLAYERBR_API AMBR_Collectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMBR_Collectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* CollisionCapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystemComponent* CollectableParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> CollectableEffect;
};
