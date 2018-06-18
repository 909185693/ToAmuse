// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "GameMode_Lobby.h"
#include "LoginServer.h"


AGameMode_Lobby::AGameMode_Lobby(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = nullptr;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameMode_Lobby::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsNetMode(NM_DedicatedServer))
	{
		TSharedPtr<TAsynTcpServer> AsynTcpServer = TAsynTcpServer::Get();
		if (AsynTcpServer.IsValid())
		{
			AsynTcpServer->Create();
		}
	}
}

void AGameMode_Lobby::Destroyed()
{
	Super::Destroyed();
}

void AGameMode_Lobby::Tick(float DeltaSeconds)
{

}
