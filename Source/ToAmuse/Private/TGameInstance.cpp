// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TGameInstance.h"
#include "Structure.h"

UTGameInstance::UTGameInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTGameInstance::Init()
{
	Super::Init();

	if (!GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		AsynTcpClient = MakeShareable(new TAsynTcpClient());
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
	if (AsynTcpClient.IsValid())
	{
		FLoginInfo LoginInfo;
		LoginInfo.Code = 1000;
		LoginInfo.Error = 3000;
		strcpy_s(LoginInfo.Message, "µÇÂ¼³É¹¦");
		strcpy_s(LoginInfo.UserName, "909185693");
		strcpy_s(LoginInfo.Password, "cc1234");

		AsynTcpClient->Send(&LoginInfo, sizeof(FLoginInfo));
	}

	return true;
}
