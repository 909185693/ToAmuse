// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ServerReceive.h"


FServerReceive::FServerReceive(TSharedPtr<TAsynTcpClient> InAsynTcpClient)
	: AsynTcpClient(InAsynTcpClient)
{
	bStopping = false;
}

FServerReceive::~FServerReceive()
{
	AsynTcpClient.Reset();
}

bool FServerReceive::Init()
{
	bStopping = false;

	return true;
}

uint32 FServerReceive::Run()
{
	if (!AsynTcpClient.IsValid())
	{
		return 0;
	}

	TArray<uint8> ReceiveData;

	uint8 Element = 0;
	
	const FTimespan WaitTime(200);

	while (!bStopping)
	{
		if (!AsynTcpClient->IsConnected())
		{
			continue;
		}

		FScopeLock* SocketLock = new FScopeLock(&AsynTcpClient->SocketCritical);
		if (FSocket* Socket = AsynTcpClient->Socket)
		{
			uint32 Size = 0;
			while (Socket->HasPendingData(Size))
			{
				ReceiveData.Init(Element, FMath::Min(Size, 65507u));

				int32 Read = 0;

				if (Socket->Recv(ReceiveData.GetData(), ReceiveData.Num(), Read) && ReceiveData.Num() > 0)
				{
					TSharedPtr<FBase> ReceiveBase = MakeShareable((FBase*)ReceiveData.GetData());

					FScopeLock* ReceiveQueueLock = new FScopeLock(&AsynTcpClient->ReceiveCritical);
					AsynTcpClient->ReceiveMessages.Enqueue(ReceiveBase);
					delete ReceiveQueueLock;
				}
				else
				{
					//AsynTcpClient->OnDisconnection(ERROR_DISCONNECTION);
				}
			}
		}
		delete SocketLock;
	}

	return 0;
}

void FServerReceive::Stop()
{
	bStopping = true;
}

void FServerReceive::Exit()
{

}