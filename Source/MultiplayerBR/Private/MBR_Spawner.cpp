// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_Spawner.h"
#include "MBR_Collectable.h"
#include "Kismet/KismetSystemLibrary.h"

//=====================================================================================================================
// Sets default values
AMBR_Spawner::AMBR_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RespawnTime = 10.f;
}

//=====================================================================================================================
// Called when the game starts or when spawned
void AMBR_Spawner::BeginPlay()
{
	Super::BeginPlay();
}

//=====================================================================================================================
void AMBR_Spawner::SetupRespawnTimer()
{
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AMBR_Spawner::RespawnCollectable, RespawnTime, false);
}

//=====================================================================================================================
void AMBR_Spawner::RespawnCollectable()
{
 		FActorSpawnParameters SpawnParams;
 		SpawnParams.Owner = this;

		FTransform SpawnTransform = GetActorTransform();
		FVector SpawnLocation = SpawnTransform.GetLocation() + FVector(0.f, 0.f, 100.f);
		SpawnTransform.SetLocation(SpawnLocation);

 		GetWorld()->SpawnActor<AMBR_Collectable>(CollectableSubclass, SpawnTransform, SpawnParams);
}

//=====================================================================================================================
void AMBR_Spawner::SetCollectableSubclass(TSubclassOf<AMBR_Collectable> SelectedSubclass)
{
	CollectableSubclass = SelectedSubclass;
}

