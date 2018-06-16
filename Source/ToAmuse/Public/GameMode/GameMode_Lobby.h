// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_Lobby.generated.h"

/**
 * ´óÌüÄ£Ê½
 */
UCLASS()
class TOAMUSE_API AGameMode_Lobby : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
	
	/**
	* Initialize the game.
	* The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	* and is used by the GameMode to initialize parameters and spawn its helper classes.
	* @warning: this is called before actors' PreInitializeComponents.
	*/
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	
};
