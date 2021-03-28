// Fill out your copyright notice in the Description page of Project Settings.


#include "MBR_GameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Interfaces/OnlineSessionInterface.h>

//=====================================================================================================================
void UMBR_GameInstance::Init()
{
	Super::Init();
	SessionNameText = TEXT("Default_Session");
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::Init Found Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::Init Found Session Interface"));
			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMBR_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMBR_GameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMBR_GameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMBR_GameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::Init No Subsystem found"));
	}
}

//=====================================================================================================================
void UMBR_GameInstance::Host()
{
	auto ExistingSession = SessionInterface->GetNamedSession(SessionNameText);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(SessionNameText);
	}
	else
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		CreateSession(SessionSettings);
	}
}

//=====================================================================================================================
void UMBR_GameInstance::CreateSession(FOnlineSessionSettings SessionSettings)
{
	auto ExistingSession = SessionInterface->GetNamedSession(SessionNameText);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(SessionNameText);
	}
	else
	{
		SessionInterface->CreateSession(0, SessionNameText, SessionSettings);
	}
}

//=====================================================================================================================
void UMBR_GameInstance::Join()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->bIsLanQuery = true;
		//SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
	
}

//=====================================================================================================================
void UMBR_GameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnCreateSessionComplete No session created"));
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnCreateSessionComplete Session created"));
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
		}
	}
}

//=====================================================================================================================
void UMBR_GameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnDestroySessionComplete Session destroyed"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, SessionNameText, SessionSettings);
}

//=====================================================================================================================
void UMBR_GameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnFindSessionsComplete Finding sessions completed"));
	if (Success && SessionSearch.IsValid() && SessionSearch->SearchResults.Num() >0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnFindSessionsComplete Sessions found!"));
		int SessionIndex = 0;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnFindSessionsComplete Session %d found!"), SessionIndex++);
		}

		SessionInterface->JoinSession(0, SessionNameText, SessionSearch->SearchResults[0]);
	}
}

//=====================================================================================================================
void UMBR_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("UMBR_GameInstance::OnJoinSessionComplete Session joined!"));
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	
}
