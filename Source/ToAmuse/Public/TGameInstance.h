// Copyright 2018 by NiHongjian. All Rights Reserved.

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

	virtual bool Tick(float DeltaSeconds);

public:
	UPROPERTY(Config)
	FString ServerIP;

	UPROPERTY(Config)
	int32 ServerPort;

protected:
	/** Delegate for callbacks to Tick */
	FTickerDelegate TickDelegate;

	/** Handle to various registered delegates */
	FDelegateHandle TickDelegateHandle;

protected:
	void NetworkProcess();
};
