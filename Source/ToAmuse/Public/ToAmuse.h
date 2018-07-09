// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Net/UnrealNetwork.h"

#if true
#define OnScreenDebugMessage(Key, DebugMessage) if(GEngine) GEngine->AddOnScreenDebugMessage(Key, 10.f, FColor::Yellow, DebugMessage, false)
#elif
#define OnScreenDebugMessage(Key, DebugMessage)
#endif
