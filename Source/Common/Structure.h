// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Macro.h"


#pragma pack(push, 1)
struct FBase
{
	FBase()
		: Code(0)
		, Error(0)
	{

	}

	FBase(int32 InCode, int32 InError)
		: Code(InCode)
		, Error(InError)
	{

	}

	int32 Code;

	int32 Error;
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

		UE_LOG(LogTemp, Warning, TEXT("FDatagram::FDatagram() InData[%d] Data[%d]"), ((FBase*)InData)->Code, ((FBase*)Data)->Code);
	}

	~FDatagram()
	{
		//delete[] Data;

		UE_LOG(LogTemp, Warning, TEXT("FDatagram::~FDatagram()"));
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