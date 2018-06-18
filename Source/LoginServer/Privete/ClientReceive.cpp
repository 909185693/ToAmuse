// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "ClientReceive.h"
#include "Structure.h"


FClientReceive::FClientReceive(TSharedPtr<TAsynTcpServer> InAsynTcpServer)
	: AsynTcpServer(InAsynTcpServer)
{

}

FClientReceive::~FClientReceive()
{
	AsynTcpServer.Reset();
}

bool FClientReceive::Init()
{
	bStopping = false;

	return true;
}

uint32 FClientReceive::Run()
{
	if (!AsynTcpServer.IsValid())
	{
		return 0;
	}

	TArray<uint8> ReceiveData;

	uint8 Element = 0;

	while (!bStopping)
	{
		FScopeLock* ClientSocketLock = new FScopeLock(&AsynTcpServer->ClientsCritical);
		for (FSocket* Socket : AsynTcpServer->Clients)
		{
			if (Socket != nullptr)
			{
				uint32 Size = 0;
				while (Socket->HasPendingData(Size))
				{
					ReceiveData.Init(Element, FMath::Min(Size, 65507u));

					int32 Read = 0;
					if (Socket->Recv(ReceiveData.GetData(), ReceiveData.Num(), Read) && ReceiveData.Num() > 0)
					{
						TSharedPtr<FReceiveMessage> ReceiveMessage = MakeShareable(new FReceiveMessage(Socket, MakeShareable((FBase*)ReceiveData.GetData())));
						
						FScopeLock* ReceiveQueueLock = new FScopeLock(&AsynTcpServer->ReceiveCritical);
						AsynTcpServer->ReceiveMessages.Enqueue(ReceiveMessage);
						delete ReceiveQueueLock;
					}
				}
			}
		}
		delete ClientSocketLock;
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