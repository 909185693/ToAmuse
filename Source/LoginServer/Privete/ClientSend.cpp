// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "ClientSend.h"


FClientSend::FClientSend(FSocket* InSocket)
	: Socket(InSocket)
{

}

FClientSend::~FClientSend()
{

}

void FClientSend::Send()
{

}

bool FClientSend::Init()
{
	bStopping = false;

	return true;
}

uint32 FClientSend::Run()
{
	if (!Socket)
	{
		return 0;
	}

	while (!bStopping)
	{
//		UE_LOG(LogLoginServerModule, Warning, TEXT("FClientSend::Run()!"));
	}

	return 0;
}

void FClientSend::Stop()
{
	bStopping = true;
}

void FClientSend::Exit()
{

}