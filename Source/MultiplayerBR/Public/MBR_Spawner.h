// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBR_Spawner.generated.h"

class AMBR_Collectable;

UCLASS()
class MULTIPLAYERBR_API AMBR_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMBR_Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle RespawnTimer;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RespawnTime;

	UFUNCTION(BlueprintCallable)
	void SetupRespawnTimer();

	UFUNCTION()
	void RespawnCollectable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<AMBR_Collectable> CollectableSubclass;

	void SetCollectableSubclass(TSubclassOf<AMBR_Collectable> SelectedSubclass);
};
