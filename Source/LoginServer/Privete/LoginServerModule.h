// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ILoginServerModule.h"

#pragma once

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