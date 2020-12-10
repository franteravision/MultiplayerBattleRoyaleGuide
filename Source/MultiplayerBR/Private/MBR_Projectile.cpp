// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MultiplayerBR/MultiplayerBRCharacter.h"

// Sets default values
AMBR_Projectile::AMBR_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent >(TEXT("Static Mesh Component"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}


bool AMBR_Projectile::Multicast_IgnoreActor_Validate(AMultiplayerBRCharacter* ActorToIgnore)
{
	return true;
}

void AMBR_Projectile::Multicast_IgnoreActor_Implementation(AMultiplayerBRCharacter* ActorToIgnore)
{
	SphereComponent->IgnoreActorWhenMoving(ActorToIgnore, true);
}

// Called when the game starts or when spawned
void AMBR_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMBR_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

