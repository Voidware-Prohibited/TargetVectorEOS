#pragma once

#include "Engine/EngineTypes.h"
#include "OnlineSessionSettings.h"
#include "SessionSettings.generated.h"

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	VeryLight,
	Easy,
	Normal,
	Heavy,
	Realistic,
	VeryHeavy,
	Extreme,
	Custom,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDifficulty, EDifficulty::Count);

UENUM(BlueprintType)
enum class EAILevel : uint8
{
	VeryBasic,
	Basic,
	Normal,
	Advanced,
	Ultra,
	Custom,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAILevel, EAILevel::Count);

UENUM(BlueprintType)
enum class EPartOfDay : uint8
{
	Morning,
	Afternoon,
	Evening,
	Night,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EPartOfDay, EPartOfDay::Count);

USTRUCT(BlueprintType)
struct FTVOnlineSessionSetting
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString Data;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 ID;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString EOnlineDataAdvertisementType;
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FTVOnlineSessionSettings
{
	GENERATED_BODY()

	/** The number of publicly available connections advertised */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumPublicConnections;

	/** The number of connections that are private (invite/password) only */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumPrivateConnections;

	/** Whether this match is publicly advertised on the online service */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bShouldAdvertise;

	/** Whether joining in progress is allowed or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowJoinInProgress;

	/** This game will be lan only and not be visible to external players */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsLANMatch;

	/** Whether the server is dedicated or player hosted */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsDedicated;

	/** Whether the match should gather stats or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUsesStats;

	/** Whether the match allows invitations for this session or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowInvites;

	/** Whether to display user presence information or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUsesPresence;

	/** Whether joining via player presence is allowed or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowJoinViaPresence;

	/** Whether joining via player presence is allowed for friends only or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowJoinViaPresenceFriendsOnly;

	/** Whether the server employs anti-cheat (punkbuster, vac, etc) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAntiCheatProtected;

	/** Whether to prefer lobbies APIs if the platform supports them */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUseLobbiesIfAvailable;

	/** Whether to create (and auto join) a voice chat room for the lobby, if the platform supports it */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUseLobbiesVoiceChatIfAvailable;

	/** Used to keep different builds from seeing each other during searches */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 BuildUniqueId;

	/** Array of custom session settings */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TMap<FName, FTVOnlineSessionSetting> Settings;
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FTVOnlineSession
{
	GENERATED_BODY()

	/** Owner of the session */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString OwningUserId;

	/** Owner name of the session */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString OwningUserName;

	/** The settings associated with this session */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FTVOnlineSessionSettings SessionSettings;

	/** The platform specific session information */
	// UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	// TSharedPtr<class FOnlineSessionInfo> SessionInfo;

	/** The number of private connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumOpenPrivateConnections;

	/** The number of publicly available connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumOpenPublicConnections;
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FTVSesssionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMin{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMax{ 100 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FDateTime DateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int Difficulty{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int AILevel{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Private{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool HasPassword{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Locked{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool AllowJoinInProgress{ true };
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FSesssionFilter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Favorite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Difficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText AILevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool HasPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Locked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float PingMax;
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FSessionSearchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 SearchResultIndex;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FTVOnlineSession Session;

	/** Owner of the session */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString OwningUserId;

	/** Owner name of the session */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString OwningUserName;

	/** The settings associated with this session */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FTVOnlineSessionSettings SessionSettings;

	/** The platform specific session information */
	// UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	// FOnlineSessionInfo SessionInfo;

	/** The number of private connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumOpenPrivateConnections;

	/** The number of publicly available connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumOpenPublicConnections;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 PingInMs;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool IsValid;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool IsSessionInfoValid;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString SessionIdStr;
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText LevelDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<TSoftObjectPtr<UTexture2D>> LevelImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool New;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Featured;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Destructable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Events;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int RecommendedPlayersMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int RecommendedPlayersMax;
};