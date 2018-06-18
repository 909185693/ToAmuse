// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "AsynTcpClient.h"
#include "ServerReceive.h"
#include "ServerSend.h"


FString TAsynTcpClient::Description = TEXT("AsynTcpServer");

TAsynTcpClient::TAsynTcpClient()
	: Socket(nullptr)
{

}

TAsynTcpClient::~TAsynTcpClient()
{
	FScopeLock* SocketLock = new FScopeLock(&SocketCritical);
	if (Socket != nullptr)
	{
		Socket->Close();
		Socket = nullptr;
	}
	delete SocketLock;
}

TSharedPtr<TAsynTcpClient> TAsynTcpClient::Get()
{
	if (!Instance.IsValid())
	{
		Instance = MakeShareable(new TAsynTcpClient());
	}

	return Instance;
}

FSocket* TAsynTcpClient::Connect(const FString& IP, int32 Port)
{
	if (Socket == nullptr)
	{
		FIPv4Address IPv4Address;
		FIPv4Address::Parse(IP, IPv4Address);

		TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		InternetAddr->SetIp(IPv4Address.Value);
		InternetAddr->SetPort(Port);

		Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, Description, false);
		if (Socket && Socket->Connect(*InternetAddr))
		{
			ReceiveThread = FRunnableThread::Create(new FServerReceive(Get()), TEXT("ClientReceive"));
			SendThread = FRunnableThread::Create(new FServerSend(Get()), TEXT("ClientSend"));

			UE_LOG(LogClientNetworkModule, Warning, TEXT("Client connect server success!"));
		}
		else
		{
			UE_LOG(LogClientNetworkModule, Warning, TEXT("Client connect server failed!"));
		}
	}
	
	return Socket;
}

void TAsynTcpClient::Send(const void* Data, int32 DataSize)
{
	FScopeLock* QueueLock = new FScopeLock(&SendCritical);
	SendMessages.Enqueue(MakeShareable(new FDatagram((uint8*)Data, DataSize)));
	delete QueueLock;
}

void TAsynTcpClient::Read(TSharedPtr<FBase>& Data)
{
	FScopeLock* ReceiveQueueLock = new FScopeLock(&ReceiveCritical);
	if (!ReceiveMessages.IsEmpty())
	{
		ReceiveMessages.Dequeue(Data);
	}
	delete ReceiveQueueLock;
}

TSharedPtr<TAsynTcpClient> TAsynTcpClient::Instance = nullptr;