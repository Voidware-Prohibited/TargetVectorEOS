// MIT


#include "TVEOSGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Kismet/GameplayStatics.h"

const FName EOSSessionName = FName(TEXT("EOSGameSession"));

UTVEOSGameInstance::UTVEOSGameInstance()
{
	bIsLoggedIn = false;
}

void UTVEOSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

bool UTVEOSGameInstance::IsLoggedIn() const
{
	return bIsLoggedIn;
}

void UTVEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials AccountCredentials;
			AccountCredentials.Type = TEXT("accountportal");

			Identity->OnLoginCompleteDelegates->AddUObject(this, &UTVEOSGameInstance::OnLoginComplete);
			Identity->Login(0, AccountCredentials);
		}
	}
}

void UTVEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Login Success: %d"), bWasSuccessful);
	bIsLoggedIn = bWasSuccessful;

	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
	}
}

void UTVEOSGameInstance::CreateSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;

				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("EOS_Lobby_TargetVector"),
					EOnlineDataAdvertisementType::ViaOnlineService);

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnCreateSessionComplete);
				SessionPtr->CreateSession(0, EOSSessionName, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Connot create session, not logged in !!!"));
	}
}

void UTVEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Successfully created session: %d"), bWasSuccessful);

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
			GetWorld()->ServerTravel(FString("L_EOSEntry?listen"));
		}
	}
}

void UTVEOSGameInstance::DestroySession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnDestroySessionComplete);
				SessionPtr->DestroySession(EOSSessionName);
			}
		}
	}
}

void UTVEOSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Successfully destroyed session: %d"), bWasSuccessful);

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
		}
	}
}

void UTVEOSGameInstance::FindFirstSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->MaxSearchResults = 5000;
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("EOS_Lobby_TargetVector"),
					EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true,
					EOnlineComparisonOp::Equals);
				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnFindFirstSessionComplete);
				SessionPtr->FindSessions(0, SearchSettings.ToSharedRef());
			}
		}
	}
}

void UTVEOSGameInstance::OnFindFirstSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully found %d Sessions !!!"), SearchSettings->SearchResults.Num());
	}

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			if (SearchSettings->SearchResults.Num() > 0)
			{
				SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnJoinSessionComplete);
				SessionPtr->JoinSession(0, EOSSessionName, SearchSettings->SearchResults[0]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Sessions found !!! :c"));
			}
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}
	}
}

void UTVEOSGameInstance::FindAllSessions()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->MaxSearchResults = 5000;
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("EOS_Lobby_TargetVector"),
					EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true,
					EOnlineComparisonOp::Equals);
				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnFindAllSessionsComplete);
				SessionPtr->FindSessions(0, SearchSettings.ToSharedRef());
			}
		}
	}
}

void UTVEOSGameInstance::OnFindAllSessionsComplete(bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			if (SearchSettings->SearchResults.Num() > 0)
			{
				SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnJoinSessionComplete);;
				for (FOnlineSessionSearchResult Session : SearchSettings->SearchResults)
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Session: %s"), *Session.GetSessionIdStr());
				}
				this->Sessions = SearchSettings->SearchResults;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Sessions found !!! :c"));
			}
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}
	}
}

void UTVEOSGameInstance::JoinSessionByIndex(int32 SessionIndex)
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(
					this, &UTVEOSGameInstance::OnJoinSessionComplete);
				SessionPtr->JoinSession(0, EOSSessionName, SearchSettings->SearchResults[SessionIndex]);
			}
		}
	}
}

void UTVEOSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(SessionName, ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
}

void UTVEOSGameInstance::GetAllFriends()
{
	if (bIsLoggedIn && OnlineSubsystem)
	{
		if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
		{
			FriendsPtr->ReadFriendsList(0, FString(""),
				FOnReadFriendsListComplete::CreateUObject(
					this, &UTVEOSGameInstance::OnGetAllFriendsComplete));
		}
	}
}

void UTVEOSGameInstance::OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName,
	const FString& ErrorStr)
{
	if (OnlineSubsystem)
	{
		if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
		{
			TArray<TSharedRef<FOnlineFriend>> FriendsList;
			if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
			{
				for (TSharedRef<FOnlineFriend> Friend : FriendsList)
				{
					FString FriendName = Friend.Get().GetRealName();
					UE_LOG(LogTemp, Warning, TEXT("Friend: %s"), *FriendName);
				}
			}
		}
	}
}

void UTVEOSGameInstance::ShowFriendsList()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowFriendsUI(0);
			}
		}
	}
}

void UTVEOSGameInstance::ShowInviteUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowInviteUI(0, EOSSessionName);
			}
		}
	}
}

int32 UTVEOSGameInstance::GetNumOfSessions()
{
	FindAllSessions();
	return this->Sessions.Num();
}

FString UTVEOSGameInstance::GetSessionID(int32 SessionIndex)
{
	FindAllSessions();
	return this->Sessions[SessionIndex].GetSessionIdStr();
}

FString UTVEOSGameInstance::GetSessionOwnerName(int32 SessionIndex)
{
	FindAllSessions();
	return this->Sessions[SessionIndex].Session.OwningUserName;
}

int32 UTVEOSGameInstance::GetSessionPing(int32 SessionIndex)
{
	FindAllSessions();
	return this->Sessions[SessionIndex].PingInMs;
}

int32 UTVEOSGameInstance::GetSessionMaxSlots(int32 SessionIndex)
{
	FindAllSessions();
	return this->Sessions[SessionIndex].Session.NumOpenPublicConnections;
}

FString UTVEOSGameInstance::GetPlayerName()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials AccountCredentials;
			AccountCredentials.Type = TEXT("accountportal");

			return Identity->GetPlayerNickname(0);
		}
	}
	return FString("");
}
