// MIT


#include "TVEOSGameInstance.h"

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

	// OnlineSubsystem = IOnlineSubsystem::Get();
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

	}
}

void UTVEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Login Success: %d"), bWasSuccessful);
	bIsLoggedIn = bWasSuccessful;

	if (OnlineSubsystem)
	{
		//if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		//{
		//	Identity->ClearOnLoginCompleteDelegates(0, this);
		//}
	}
}



void UTVEOSGameInstance::ShowFriendsList()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{

		}
	}
}

void UTVEOSGameInstance::ShowInviteUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{

		}
	}
}

FString UTVEOSGameInstance::GetPlayerName()
{
	if (OnlineSubsystem)
	{

	}
	return FString("");
}
