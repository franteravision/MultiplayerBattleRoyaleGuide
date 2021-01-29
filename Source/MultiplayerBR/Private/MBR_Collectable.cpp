// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_Collectable.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

//=====================================================================================================================
// Sets default values
AMBR_Collectable::AMBR_Collectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule Component"));
	RootComponent = CollisionCapsuleComponent;

	CollectableParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Collectable Particle System"));
	CollectableParticleSystem->SetupAttachment(RootComponent);

	bReplicates = true;
}

//=====================================================================================================================
// Called when the game starts or when spawned
void AMBR_Collectable::BeginPlay()
{
	Super::BeginPlay();
}
