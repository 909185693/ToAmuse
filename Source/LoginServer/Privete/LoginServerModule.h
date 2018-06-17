// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "ILoginServerModule.h"


DECLARE_LOG_CATEGORY_EXTERN(LogLoginServerModule, Log, All);

/**
* Implements the LoginServer module.
*/
class FLoginServerModule
	: public ILoginServerModule
{
public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual bool IsGameModule() const override;
};