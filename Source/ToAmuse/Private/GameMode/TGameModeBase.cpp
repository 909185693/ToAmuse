// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TGameModeBase.h"
#include "THUD.h"
#include "TCharacter.h"
#include "TPlayerController.h"


ATGameModeBase::ATGameModeBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ATCharacter::StaticClass();

	PlayerControllerClass = ATPlayerController::StaticClass();

	HUDClass = ATHUD::StaticClass();
}

