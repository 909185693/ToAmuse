// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ServerSend.h"


FServerSend::FServerSend(TSharedPtr<TAsynTcpClient> InAsynTcpClient)
	: AsynTcpClient(InAsynTcpClient)
{

}

FServerSend::~FServerSend()
{
	AsynTcpClient.Reset();
}

bool FServerSend::Init()
{
	bStopping = false;

	return true;
}

uint32 FServerSend::Run()
{
	if (!AsynTcpClient.IsValid())
	{
		return 0;
	}

	const FTimespan WaitTime(200);

	while (!bStopping)
	{
		if (!AsynTcpClient->IsConnected())
		{
			continue;
		}

		FScopeLock* QueueLock = new FScopeLock(&AsynTcpClient->SendCritical);
		if (!AsynTcpClient->SendMessages.IsEmpty())
		{
			TSharedPtr<FDatagram> Datagram;
			AsynTcpClient->SendMessages.Dequeue(Datagram);

			if (Datagram.IsValid())
			{
				FScopeLock* SocketLock = new FScopeLock(&AsynTcpClient->SocketCritical);
				if (FSocket* Socket = AsynTcpClient->Socket)
				{
					int32 Sent = 0;

					if (Socket->Send(Datagram->Data, Datagram->Size, Sent))
					{
						UE_LOG(LogClientNetworkModule, Warning, TEXT("FServerSend::Run() Success!"));
					}
					else
					{
						UE_LOG(LogClientNetworkModule, Warning, TEXT("FServerSend::Run() Failed!"));
					}
				}
				delete SocketLock;

				Datagram.Reset();
			}
		}
		delete QueueLock;
	}

	return 0;
}

void FServerSend::Stop()
{
	bStopping = true;
}

void FServerSend::Exit()
{

}