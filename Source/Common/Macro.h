// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once


UENUM(BlueprintType)
enum ENetworkErrorCode
{
#ifndef EXPRESS_ERROR_CODE
#define EXPRESS_ERROR_CODE(Macro, Value, Explain) \
	Macro = Value UMETA(DisplayName = Explain),
#include "ErrorCode.h"
#endif // EXPRESS_ERROR_CODE
};

UENUM(BlueprintType)
enum ENetworkLogicCode
{
#ifndef EXPRESS_LOGIC_CODE
#define EXPRESS_LOGIC_CODE(Macro, Value, Explain) \
	Macro=Value UMETA(DisplayName = Explain),
#include "LogicCode.h"
#endif // EXPRESS_LOGIC_CODE
};


