// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "TGameInstance.generated.h"


/**
* UTGameInstance
*/
UCLASS(config=Game)
class TOAMUSE_API UTGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
	
	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;
	virtual void Shutdown() override;
};
