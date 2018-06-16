// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "ClientReceive.h"


FClientReceive::FClientReceive(FSocket* InSocket)
	: Socket(InSocket)
{
	bStopping = false;
}

FClientReceive::~FClientReceive()
{

}

void FClientReceive::Receive()
{

}

bool FClientReceive::Init()
{
	bStopping = false;

	return true;
}

uint32 FClientReceive::Run()
{
	if (!Socket)
	{
		return 0;
	}

	while (!bStopping)
	{
//		UE_LOG(LogLoginServerModule, Warning, TEXT("FClientReceive::Run()!"));
	}

	return 0;
}

void FClientReceive::Stop()
{
	bStopping = true;
}

void FClientReceive::Exit()
{

}