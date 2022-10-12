// MIT


#include "TVEOSGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/SessionSettings.h"

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

void UTVEOSGameInstance::CreateSession(FTVSesssionSettings EOSSessionSettings)
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;

				SessionSettings.bIsDedicated = false;
				// SessionSettings.bShouldAdvertise = true;
				SessionSettings.bShouldAdvertise = EOSSessionSettings.Private;
				SessionSettings.bIsLANMatch = false;
				// SessionSettings.NumPublicConnections = 5;
				SessionSettings.NumPublicConnections = EOSSessionSettings.PlayersMax;
				// SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinInProgress = EOSSessionSettings.Locked;
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
					this, &UTVEOSGameInstance::OnJoinSessionComplete);
				int index{ -1 };
				for (FOnlineSessionSearchResult Session : SearchSettings->SearchResults)
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Session: %s"), *Session.GetSessionIdStr());
					index = ++index;
					FSessionSearchResult SearchResultLocal;
					FTVOnlineSession OnlineSessionLocal;
					FTVOnlineSessionSettings OnlineSessionSettingsLocal;

					//FOnlineSessionSearchResult

					/** Index ref of the session in SearchResults */
					SearchResultLocal.SearchResultIndex = index;

					// SESSION FOnlineSession -> FTVOnlineSession
					/** Owner ID of the session */
					//SearchResultLocal.Session.OwningUserId = Session.Session.OwningUserId;

					/** Owner name of the session */
					SearchResultLocal.Session.OwningUserName = Session.Session.OwningUserName;

					//SESSIONSETTINGS FOnlineSessionSettings -> FTVOnlineSessionSettings

					/** The number of publicly available connections advertised */
					SearchResultLocal.Session.SessionSettings.NumPublicConnections = Session.Session.SessionSettings.NumPublicConnections;

					/** The number of connections that are private (invite/password) only */
					SearchResultLocal.Session.SessionSettings.NumPrivateConnections = Session.Session.SessionSettings.NumPrivateConnections;

					/** Whether this match is publicly advertised on the online service */
					SearchResultLocal.Session.SessionSettings.bShouldAdvertise = Session.Session.SessionSettings.bShouldAdvertise;

					/** Whether joining in progress is allowed or not */
					SearchResultLocal.Session.SessionSettings.bAllowJoinInProgress = Session.Session.SessionSettings.bAllowJoinInProgress;

					/** This game will be lan only and not be visible to external players */
					SearchResultLocal.Session.SessionSettings.bIsLANMatch = Session.Session.SessionSettings.bIsLANMatch;

					/** Whether the server is dedicated or player hosted */
					SearchResultLocal.Session.SessionSettings.bIsDedicated = Session.Session.SessionSettings.bIsDedicated;

					/** Whether the match should gather stats or not */
					SearchResultLocal.Session.SessionSettings.bUsesStats = Session.Session.SessionSettings.bUsesStats;

					/** Whether the match allows invitations for this session or not */
					SearchResultLocal.Session.SessionSettings.bAllowInvites = Session.Session.SessionSettings.bAllowInvites;

					/** Whether to display user presence information or not */
					SearchResultLocal.Session.SessionSettings.bUsesPresence = Session.Session.SessionSettings.bUsesPresence;

					/** Whether joining via player presence is allowed or not */
					SearchResultLocal.Session.SessionSettings.bAllowJoinViaPresence = Session.Session.SessionSettings.bAllowJoinViaPresence;

					/** Whether joining via player presence is allowed for friends only or not */
					SearchResultLocal.Session.SessionSettings.bAllowJoinViaPresenceFriendsOnly = Session.Session.SessionSettings.bAllowJoinViaPresenceFriendsOnly;

					/** Whether the server employs anti-cheat (punkbuster, vac, etc) */
					SearchResultLocal.Session.SessionSettings.bAntiCheatProtected = Session.Session.SessionSettings.bAntiCheatProtected;

					/** Whether to prefer lobbies APIs if the platform supports them */
					SearchResultLocal.Session.SessionSettings.bUseLobbiesIfAvailable = Session.Session.SessionSettings.bUseLobbiesIfAvailable;

					/** Whether to create (and auto join) a voice chat room for the lobby, if the platform supports it */
					SearchResultLocal.Session.SessionSettings.bUseLobbiesVoiceChatIfAvailable = Session.Session.SessionSettings.bUseLobbiesVoiceChatIfAvailable;

					/** Used to keep different builds from seeing each other during searches */
					SearchResultLocal.Session.SessionSettings.BuildUniqueId = Session.Session.SessionSettings.BuildUniqueId;

					/** Array of custom session settings */
					//FOR LOOP
					//SearchResultLocal.Session.SessionSettings.Settings = Session.Session.SessionSettings.Settings;

					TArray<FName> KeyArray {};
					//TArray<UObject*> ValueArray;
					TArray<FString> ValueArray {};
					TMap<FName, FString> SessionSettingsLocal = {};
					SearchResultLocal.Session.SessionSettings.Settings.GenerateKeyArray(KeyArray);

					for(auto i : KeyArray){
					
						// SearchResultLocal.Session.SessionSettings.Settings = Session.Session.SessionSettings.Settings.GenerateValueArray(ValueArray);
						//SearchResultLocal.Session.SessionSettings.Settings.FindRef(i).ToString();
						FString ValueData;
						ValueData = Session.Session.SessionSettings.Settings.FindRef(i).Data.ToString();
						ValueArray.Add(ValueData);
					}

					for (auto i = 0; i < KeyArray.Num(); i++) {
						SessionSettingsLocal[KeyArray[i]] = ValueArray[i];
					}

					/** The platform specific session information */
					// TSharedPtr<class FOnlineSessionInfo> SessionInfo;

					/** The number of private connections that are available (read only) */
					SearchResultLocal.Session.NumOpenPrivateConnections = Session.Session.NumOpenPrivateConnections;

					/** The number of publicly available connections that are available (read only) */
					SearchResultLocal.Session.NumOpenPublicConnections = Session.Session.NumOpenPublicConnections;
					
					// FSessionSearchResult
					// FTVOnlineSession
					// FTVOnlineSessionSettings
					// FTVSesssionSettings

					//FOnlineSessionSearchResult

					SearchResultLocal.PingInMs = Session.PingInMs;
					SearchResultLocal.IsValid = Session.IsValid();
					SearchResultLocal.IsSessionInfoValid = Session.IsSessionInfoValid();
					SearchResultLocal.SessionIdStr = Session.GetSessionIdStr();

					SearchResults.Add(SearchResultLocal);
					// SearchResults
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

// void UTVEOSGameInstance::GetSessions(Array<FOnlineSessionSearchResult*>& Sessions)
// {
// 	return Sessions;
// }

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
