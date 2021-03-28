// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <UObject/NameTypes.h>
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include "MBR_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBR_API UMBR_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	IOnlineSessionPtr SessionInterface;

	FName SessionNameText;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void Host();

	UFUNCTION(BlueprintCallable)
	void Join();
	
	void CreateSession(FOnlineSessionSettings SessionSettings);

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
