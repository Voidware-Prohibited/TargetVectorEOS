#pragma once

#include "Engine/EngineTypes.h"
#include "NativeGameplayTags.h"
#include "SessionSettings.generated.h"

UENUM(BlueprintType)
enum class EInstanceState : uint8
{
	Loading	UMETA(DisplayName = "Loading"),
	MainMenu	UMETA(DisplayName = "Main Menu"),
	InGame	UMETA(DisplayName = "In Game"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EInstanceState, EInstanceState::Count);

UENUM(BlueprintType)
enum class ESessionType : uint8
{
	SinglePlayer	UMETA(DisplayName = "Single Player"),
	Coop	UMETA(DisplayName = "Co-Op"),
	Multiplayer	UMETA(DisplayName = "Multiplayer"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESessionType, ESessionType::Count);

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	VeryLight	UMETA(DisplayName = "Very Light"),
	Easy	UMETA(DisplayName = "Easy"),
	Normal	UMETA(DisplayName = "Normal"),
	Heavy	UMETA(DisplayName = "Heavy"),
	Realistic	UMETA(DisplayName = "Realistic"),
	VeryHeavy	UMETA(DisplayName = "Very Heavy"),
	Extreme	UMETA(DisplayName = "Extreme"),
	Custom	UMETA(DisplayName = "Custom"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDifficulty, EDifficulty::Count);

UENUM(BlueprintType)
enum class EAILevel : uint8
{
	VeryBasic	UMETA(DisplayName = "Very Basic"),
	Basic	UMETA(DisplayName = "Basic"),
	Normal	UMETA(DisplayName = "Normal"),
	Advanced	UMETA(DisplayName = "Advanced"),
	Ultra	UMETA(DisplayName = "Ultra"),
	Custom	UMETA(DisplayName = "Custom"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAILevel, EAILevel::Count);

UENUM(BlueprintType)
enum class EPartOfDay : uint8
{
	Morning	UMETA(DisplayName = "Morning"),
	Afternoon	UMETA(DisplayName = "Afternoon"),
	Evening	UMETA(DisplayName = "Evening"),
	Night	UMETA(DisplayName = "Night"),
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
	int32 ID {0};

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString EOnlineDataAdvertisementType;
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FTVOnlineSessionSettings
{
	GENERATED_BODY()

	/** The number of publicly available connections advertised */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumPublicConnections {0};

	/** The number of connections that are private (invite/password) only */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumPrivateConnections {0};

	/** Whether this match is publicly advertised on the online service */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bShouldAdvertise {true};

	/** Whether joining in progress is allowed or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowJoinInProgress {true};

	/** This game will be lan only and not be visible to external players */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsLANMatch {false};

	/** Whether the server is dedicated or player hosted */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsDedicated {false};

	/** Whether the match should gather stats or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUsesStats {true};

	/** Whether the match allows invitations for this session or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowInvites {true};

	/** Whether to display user presence information or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUsesPresence {true};

	/** Whether joining via player presence is allowed or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowJoinViaPresence {true};

	/** Whether joining via player presence is allowed for friends only or not */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAllowJoinViaPresenceFriendsOnly {false};

	/** Whether the server employs anti-cheat (punkbuster, vac, etc) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bAntiCheatProtected {false};

	/** Whether to prefer lobbies APIs if the platform supports them */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUseLobbiesIfAvailable {true};

	/** Whether to create (and auto join) a voice chat room for the lobby, if the platform supports it */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bUseLobbiesVoiceChatIfAvailable {true};

	/** Used to keep different builds from seeing each other during searches */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 BuildUniqueId {0};

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
	int32 NumOpenPrivateConnections {0};

	/** The number of publicly available connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumOpenPublicConnections {0};
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FTVSesssionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Base Game Mode", AllowPrivateAccess))
	FGameplayTag BaseGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMin {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMax {100};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FDateTime DateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int Difficulty {2};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int AILevel {2};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Private {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool HasPassword {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Locked {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool AllowJoinInProgress{ true };
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FSesssionFilter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Favorite {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMin {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int PlayersMax {100};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Difficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText AILevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool HasPassword {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Locked {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float PingMax {0.0f};
};

USTRUCT(BlueprintType)
struct TARGETVECTOREOS_API FSessionSearchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 SearchResultIndex {0};

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
	int32 NumOpenPrivateConnections {0};

	/** The number of publicly available connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 NumOpenPublicConnections {0};

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 PingInMs {0};

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool IsValid {false};

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool IsSessionInfoValid {false};

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
	bool New {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Featured {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Destructable {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Events {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int RecommendedPlayersMin {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int RecommendedPlayersMax {0};
};