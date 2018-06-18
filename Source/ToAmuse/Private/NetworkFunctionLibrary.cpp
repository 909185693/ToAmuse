// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "NetworkFunctionLibrary.h"
#include "TGameInstance.h"
#include "LogicCode.h"

UNetworkFunctionLibrary::UNetworkFunctionLibrary()
{

}

UNetworkFunctionLibrary::~UNetworkFunctionLibrary()
{
	Instance = nullptr;
}

UNetworkFunctionLibrary* UNetworkFunctionLibrary::Get(UObject* WorldContextObject)
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UNetworkFunctionLibrary>();
	}

	return Instance;
}

bool UNetworkFunctionLibrary::Connect(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	UTGameInstance* GameInstance = World ? Cast<UTGameInstance>(World->GetGameInstance()) : nullptr;
	if (GameInstance != nullptr)
	{
		TSharedPtr<TAsynTcpClient> AsynTcpClient = TAsynTcpClient::Get();
		if (AsynTcpClient.IsValid())
		{
			return AsynTcpClient->Connect(GameInstance->ServerIP, GameInstance->ServerPort) != nullptr;
		}
	}

	return false;
}

void UNetworkFunctionLibrary::Login(UObject* WorldContextObject, const FText& UserName, const FText& Password)
{
	TSharedPtr<TAsynTcpClient> AsynTcpClient = TAsynTcpClient::Get();
	if (AsynTcpClient.IsValid())
	{
		if (AsynTcpClient.IsValid())
		{
			FLoginInfo LoginInfo;
			LoginInfo.Code = USER_LOGIN;
			FCString::Strcpy(LoginInfo.UserName, *UserName.ToString());
			FCString::Strcpy(LoginInfo.Password, *Password.ToString());

			AsynTcpClient->Send(&LoginInfo, sizeof(FLoginInfo));
		}
	}
}

class UNetworkFunctionLibrary* UNetworkFunctionLibrary::Instance = nullptr;