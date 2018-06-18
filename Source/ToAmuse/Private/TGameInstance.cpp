// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TGameInstance.h"
#include "ClientNetwork.h"
#include "Structure.h"
#include "NetworkFunctionLibrary.h"


UTGameInstance::UTGameInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTGameInstance::Init()
{
	Super::Init();

	if (!GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		TSharedPtr<TAsynTcpClient> AsynTcpClient = TAsynTcpClient::Get();		
		if (AsynTcpClient.IsValid())
		{
			AsynTcpClient->Connect(ServerIP, ServerPort);
		}
	}

	// Register delegate for ticker callback
	TickDelegate = FTickerDelegate::CreateUObject(this, &UTGameInstance::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void UTGameInstance::Shutdown()
{
	Super::Shutdown();

	// Unregister ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

bool UTGameInstance::Tick(float DeltaSeconds)
{
	NetworkProcess();

	return true;
}

void UTGameInstance::NetworkProcess()
{
	TSharedPtr<FBase> Data;
	TAsynTcpClient::Get()->Read(Data);
	if (Data.IsValid())
	{
		switch (Data->Code)
		{
		case USER_LOGIN:
			UNetworkFunctionLibrary::Get(this)->OnLogin.Broadcast(Data->Error);
			break;
		default:
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("Code[%d] Error[%d]"), Data->Code, Data->Error);
	}
}
