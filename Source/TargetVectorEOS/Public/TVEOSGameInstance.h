// MIT

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

	UFUNCTION(BlueprintCallable, Category = "Game Instance|Player")
	FString GetPlayerName();

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn;
};
