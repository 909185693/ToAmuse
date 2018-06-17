// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Engine.h"

#pragma pack(push, 1)
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

struct FBase
{
	FBase()
		: Code(0)
		, Error(0)
	{

	}

	FBase(uint16 InCode, uint16 InError)
		: Code(InCode)
		, Error(InError)
	{

	}

	uint16 Code;

	uint16 Error;

	char Message[32];
};

struct FLoginInfo : public FBase
{
	char UserName[64];

	char Password[64];
};

#pragma pack(pop)