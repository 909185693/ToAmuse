// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ServerSend.h"


FServerSend::FServerSend(TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> InAsynTcpClient)
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
		if (!AsynTcpClient->SendMessages.IsEmpty())
		{
			TSharedPtr<FDatagram, ESPMode::ThreadSafe> Datagram;
			AsynTcpClient->SendMessages.Dequeue(Datagram);

			if (Datagram.IsValid())
			{
				TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket = AsynTcpClient->Socket;
				if (Socket.IsValid())
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

				Datagram.Reset();
			}
		}
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