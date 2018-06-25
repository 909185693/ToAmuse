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

	while (bHostConnected && !bStopping)
	{
		TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket = AsynTcpClient->Socket;
		if (Socket.IsValid())
		{
			if (Socket->Wait(ESocketWaitConditions::WaitForReadOrWrite, FTimespan(200)))
			{
				bHostConnected = true;
				AsynTcpClient->OnConnect();
			}
			else if (++ConnectedFailedNum == MAX_CONNECT_FAILED_NUM)
			{
				ConnectedFailedNum = 0;

				AsynTcpClient->OnDisconnection(ERROR_DISCONNECTION);
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