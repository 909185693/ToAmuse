// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "Engine.h"
#include "IClientNetworkModule.h"

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogClientNetworkModule, Log, All);

/**
* Implements the LoginServer module.
*/
class FClientNetworkModule
	: public IClientNetworkModule
{
public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual bool IsGameModule() const override;
};