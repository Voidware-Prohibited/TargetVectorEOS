// MIT

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Settings/SessionSettings.h"
#include "TVEOSGameInstance.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FTVEOSSessionSettingsInput
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDedicated{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldAdvertise{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLANMatch{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NumPublicConnections{ 5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAllowJoinInProgress{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAllowJoinViaPresence{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUsesPresence{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseLobbiesIfAvailable{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName GameName {
		"EOS_Lobby_TargetVector"
	};
};

UCLASS()
class TARGETVECTOREOS_API UTVEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTVEOSGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	bool IsLoggedIn() const;

	UFUNCTION(BlueprintCallable)
	void Login();

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSesssionFilter PlayerSesssionFilter;

	UFUNCTION(BlueprintCallable)
	void CreateSession(FTVSesssionSettings EOSSessionSettings);

	UFUNCTION(BlueprintCallable)
		void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void FindFirstSession();
	void OnFindFirstSessionComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void FindAllSessions();
	void OnFindAllSessionsComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void JoinSessionByIndex(int32 SessionIndex);

	TSharedPtr<FOnlineSessionSearch> SearchSettings;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
		void GetAllFriends();
	void OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UFUNCTION(BlueprintCallable)
		void ShowFriendsList();

	UFUNCTION(BlueprintCallable)
		void ShowInviteUI();

	TArray<FOnlineSessionSearchResult> Sessions;

	// UFUNCTION(BlueprintCallable)
	// void GetSessions(TArray<FOnlineSessionSearchResult> &Sessions);

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TArray<FSessionSearchResult> SearchResults;

	UFUNCTION(BlueprintCallable)
	int32 GetNumOfSessions();

	UFUNCTION(BlueprintCallable)
		FString GetSessionID(int32 SessionIndex);

	UFUNCTION(BlueprintCallable)
		FString GetSessionOwnerName(int32 SessionIndex);

	UFUNCTION(BlueprintCallable)
		FString GetPlayerName();

	UFUNCTION(BlueprintCallable)
		int32 GetSessionPing(int32 SessionIndex);

	UFUNCTION(BlueprintCallable)
		int32 GetSessionMaxSlots(int32 SessionIndex);

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn;
};
