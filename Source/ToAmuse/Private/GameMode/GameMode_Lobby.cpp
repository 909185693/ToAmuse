// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "GameMode_Lobby.h"


AGameMode_Lobby::AGameMode_Lobby(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = nullptr;
}

void AGameMode_Lobby::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,  Options, ErrorMessage)
}
