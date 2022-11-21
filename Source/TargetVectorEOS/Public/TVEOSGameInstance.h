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

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Login")
	bool IsLoggedIn() const;

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Login")
	void Login();

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Instance|Session", Meta = (AllowPrivateAccess))
	FSesssionFilter PlayerSesssionFilter;

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	void CreateSession(FTVSesssionSettings EOSSessionSettings);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	void FindFirstSession();
	void OnFindFirstSessionComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	void FindAllSessions();
	void OnFindAllSessionsComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	void JoinSessionByIndex(int32 SessionIndex);

	TSharedPtr<FOnlineSessionSearch> SearchSettings;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Friends")
	void GetAllFriends();
	void OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Friends")
	void ShowFriendsList();

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Friends")
	void ShowInviteUI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Instance|State", Meta = (AllowPrivateAccess))
	EInstanceState InstanceState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Instance|State", Meta = (AllowPrivateAccess))
	bool bIsNewInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Instance|State", Meta = (AllowPrivateAccess))
	ESessionType SessionType;

	TArray<FOnlineSessionSearchResult> Sessions;

	// UFUNCTION(BlueprintCallable)
	// void GetSessions(TArray<FOnlineSessionSearchResult> &Sessions);

	UPROPERTY(BlueprintReadOnly, Category = "Game Instance|Session", Meta = (AllowPrivateAccess))
	TArray<FSessionSearchResult> SearchResults;

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	int32 GetNumOfSessions();

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	FString GetSessionID(int32 SessionIndex);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	FString GetSessionOwnerName(int32 SessionIndex);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Player")
	FString GetPlayerName();

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	int32 GetSessionPing(int32 SessionIndex);

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Session")
	int32 GetSessionMaxSlots(int32 SessionIndex);

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn;
};
