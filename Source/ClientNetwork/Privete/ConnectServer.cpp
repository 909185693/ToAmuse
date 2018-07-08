// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ConnectServer.h"


#define MAX_CONNECT_FAILED_NUM 20

FConnectServer::FConnectServer(TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> InAsynTcpClient, const FString& InIP, int32 InPort)
	: AsynTcpClient(InAsynTcpClient)
	, bStopping(false)
	, ConnectedFailedNum(0)
	, IP(InIP)
	, Port(InPort)
{

}

FConnectServer::~FConnectServer()
{
	AsynTcpClient.Reset();

	UE_LOG(LogClientNetworkModule, Warning, TEXT("FConnectServer::~FConnectServer() callback!"));
}

bool FConnectServer::Init()
{
	bStopping = false;

	return true;
}

uint32 FConnectServer::Run()
{
	if (!AsynTcpClient.IsValid())
	{
		return 0;
	}

	const FTimespan WaitTime(200);

	while (bHostConnected && !bStopping)
	{
		FSocket*& Socket = AsynTcpClient->Socket;
		if (Socket != nullptr)
		{
			if (Socket->Wait(ESocketWaitConditions::WaitForReadOrWrite, WaitTime))
			{
				bHostConnected = true;

				AsynTcpClient->OnSocketConnect(true);
			}
			else if (++ConnectedFailedNum == MAX_CONNECT_FAILED_NUM)
			{
				ConnectedFailedNum = 0;

				AsynTcpClient->OnSocketConnect(false);
			}
		}
	}

	return 0;
}

void FConnectServer::Stop()
{
	bStopping = true;
}

void FConnectServer::Exit()
{
	UE_LOG(LogClientNetworkModule, Warning, TEXT("FConnectServer::Exit() callback!"));
}