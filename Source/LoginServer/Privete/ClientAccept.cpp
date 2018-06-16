// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "ClientAccept.h"


FClientAccept::FClientAccept(FSocket* InSocket)
	: Socket(InSocket)
{

}

FClientAccept::~FClientAccept()
{

}

void FClientAccept::Accept()
{

}

bool FClientAccept::Init()
{
	bStopping = false;

	return true;
}

uint32 FClientAccept::Run()
{
	if (!Socket)
	{
		return 0;
	}

	while (!bStopping)
	{
//		UE_LOG(LogLoginServerModule, Warning, TEXT("FClientAccept::Run()!"));
	}

	return 0;
}

void FClientAccept::Stop()
{
	bStopping = true;
}

void FClientAccept::Exit()
{

}