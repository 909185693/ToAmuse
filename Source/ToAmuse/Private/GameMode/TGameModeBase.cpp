// Copyright 2018 by January. All Rights Reserved.

#include "TGameModeBase.h"
#include "THUD.h"
#include "TPlayerController.h"
#include "ConstructorHelpers.h"


ATGameModeBase::ATGameModeBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PawnClassFinder(TEXT("/Game/Blueprints/Character/CharacterBP_Mannequin"));
	if (PawnClassFinder.Succeeded())
	{
		DefaultPawnClass = PawnClassFinder.Class;
	}

	PlayerControllerClass = ATPlayerController::StaticClass();

	HUDClass = ATHUD::StaticClass();
}

