// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "ClientAccept.h"


FClientAccept::FClientAccept(TAsynTcpServer* InAsynTcpServer)
	: AsynTcpServer(InAsynTcpServer)
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
	FSocket* Socket = AsynTcpServer ? AsynTcpServer->Socket : nullptr;
	if (!Socket)
	{
		return 0;
	}

	while (!bStopping)
	{
		FSocket* AcceptSocket = Socket->Accept(AsynTcpServer->Description);
		if (AcceptSocket != nullptr)
		{
			FScopeLock* QueueLock = new FScopeLock(&AsynTcpServer->ClientsCritical);
			AsynTcpServer->Clients.Add(AcceptSocket);
			delete QueueLock;

			UE_LOG(LogLoginServerModule, Warning, TEXT("FClientAccept::Run()!"));
		}
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