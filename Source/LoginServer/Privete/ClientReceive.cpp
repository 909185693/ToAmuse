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
		TArray<FSocket*>::TIterator It(AsynTcpServer->Clients);
		for (;It;++It)
		{
			FSocket* Socket = *It;
			if (Socket != nullptr)
			{
				uint32 Size = 0;
				
				while (Socket->HasPendingData(Size))
				{
					ReceiveData.Init(Element, FMath::Min(Size, 65507u));

					int32 Read = 0;
					if (Socket->Recv(ReceiveData.GetData(), ReceiveData.Num(), Read) && ReceiveData.Num() > 0)
					{
						TSharedPtr<FBase> Data = MakeShareable((FBase*)ReceiveData.GetData());
						TSharedPtr<FReceiveMessage> ReceiveMessage = MakeShareable(new FReceiveMessage(Socket, Data));
						
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