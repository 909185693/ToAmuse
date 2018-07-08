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

void UNetworkFunctionLibrary::Connect(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	UTGameInstance* GameInstance = World ? Cast<UTGameInstance>(World->GetGameInstance()) : nullptr;
	if (GameInstance != nullptr)
	{
		TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> AsynTcpClient = TAsynTcpClient::Get();
		if (AsynTcpClient.IsValid())
		{
			AsynTcpClient->OnConnect().BindUObject(Get(World), &UNetworkFunctionLibrary::OnSocketConnect);
			AsynTcpClient->Connect(GameInstance->ServerIP, GameInstance->ServerPort);
		}
	}
}

void UNetworkFunctionLibrary::Login(UObject* WorldContextObject, const FText& UserName, const FText& Password)
{
	TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> AsynTcpClient = TAsynTcpClient::Get();
	if (AsynTcpClient.IsValid())
	{
		if (AsynTcpClient.IsValid())
		{
			FLoginInfo LoginInfo;
			LoginInfo.Code = USER_LOGIN;
			FCString::Strcpy(LoginInfo.UserName, *UserName.ToString());
			FCString::Strcpy(LoginInfo.Password, *Password.ToString());

			TSharedRef<TArray<uint8>, ESPMode::ThreadSafe> Data = MakeShareable(new TArray<uint8>());
			FMemory::Memcmp(Data->GetData(), &LoginInfo, sizeof(FLoginInfo));

			AsynTcpClient->Send(Data);
		}
	}
}

void UNetworkFunctionLibrary::OnSocketConnect(bool bWasSuccessful)
{
	OnConnect.Broadcast(bWasSuccessful);
}

class UNetworkFunctionLibrary* UNetworkFunctionLibrary::Instance = nullptr;