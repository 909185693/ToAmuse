// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Macro.h"


#pragma pack(push, 1)
struct FBase
{
	FBase()
		: Code(LOGIC_NONE)
		, Error(ERROR_NONE)
	{

	}

	FBase(ENetworkErrorCode InError)
		: Code(LOGIC_NONE)
		, Error(InError)
	{

	}

	FBase(ENetworkLogicCode InCode)
		: Code(InCode)
		, Error(ERROR_NONE)
	{

	}

	FBase(ENetworkLogicCode InCode, ENetworkErrorCode InError)
		: Code(InCode)
		, Error(InError)
	{

	}

	ENetworkLogicCode Code;

	ENetworkErrorCode Error;
};

struct FDatagram
{
	FDatagram()
		: Data(nullptr)
		, Size(0)
	{

	}

	FDatagram(uint8* InData, int32 InSize)
	{
		Data = new uint8[InSize];
		FMemory::Memcpy(Data, InData, InSize);

		Size = InSize;
	}

	~FDatagram()
	{
		delete[] Data;
	}

	uint8* Data;

	int32 Size;
};

struct FLoginInfo : public FBase
{
	TCHAR UserName[64];

	TCHAR Password[64];
};

#pragma pack(pop)