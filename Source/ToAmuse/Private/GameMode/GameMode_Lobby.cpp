// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "GameMode_Lobby.h"


AGameMode_Lobby::AGameMode_Lobby(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = nullptr;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameMode_Lobby::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	AsynTcpServer = MakeShareable(new TAsynTcpServer());
	if (AsynTcpServer.IsValid())
	{
		AsynTcpServer->Create();
	}
}

void AGameMode_Lobby::Destroyed()
{
	AsynTcpServer.Reset();

	Super::Destroyed();
}

void AGameMode_Lobby::Tick(float DeltaSeconds)
{
	if (AsynTcpServer.IsValid())
	{
		TSharedPtr<FBase> Data;
		AsynTcpServer->Read(Data);
		if (Data.IsValid())
		{
			FLoginInfo LoginInfo = *(FLoginInfo*)Data.Get();
			
			UE_LOG(LogTemp, Log, TEXT("Code[%d] Error[%d] Username[%s] Password[%s]"), LoginInfo.Code, LoginInfo.Error, LoginInfo.UserName, LoginInfo.Password);
		}
	}
}
