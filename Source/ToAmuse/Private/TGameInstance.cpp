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
	return true;
}
