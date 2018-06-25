// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ServerReceive.h"


#define MAX_FAILD_READ_COUNT 3

FServerReceive::FServerReceive(TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> InAsynTcpClient)
	: AsynTcpClient(InAsynTcpClient)
	, bStopping(false)
	, FailedReadCount(0)
{

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
	
	ReceiveData.Init(0.f, 65507u);

	const FTimespan WaitTime(200);
	
	while (!bStopping)
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket = AsynTcpClient->Socket;
		if (Socket.IsValid())
		{
			if (Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
			{
				int32 Read = 0;

				Socket->Recv(ReceiveData.GetData(), ReceiveData.Num(), Read);

				if (Read > 0)
				{
					TSharedPtr<FBase, ESPMode::ThreadSafe> ReceiveBase = MakeShareable((FBase*)ReceiveData.GetData());

					AsynTcpClient->ReceiveMessages.Enqueue(ReceiveBase);
				}
				else if (++FailedReadCount >= MAX_FAILD_READ_COUNT)
				{
					FailedReadCount = 0;

					AsynTcpClient->OnDisconnection(ERROR_DISCONNECTION);
				}
			}
		}
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