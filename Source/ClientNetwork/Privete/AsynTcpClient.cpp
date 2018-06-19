// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "AsynTcpClient.h"
#include "ServerReceive.h"
#include "ServerSend.h"


FString TAsynTcpClient::Description = TEXT("AsynTcpServer");

TAsynTcpClient::TAsynTcpClient()
	: Socket(nullptr)
	, ReceiveThread(nullptr)
	, SendThread(nullptr)
	, bIsConnected(false)
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
		Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, Description, false);
		//Socket->SetNonBlocking();
	}

	FIPv4Address IPv4Address;
	FIPv4Address::Parse(IP, IPv4Address);

	TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(IPv4Address.Value);
	InternetAddr->SetPort(Port);

	if (Socket && Socket->Connect(*InternetAddr))
	{
		bIsConnected = true;

		if (ReceiveThread == nullptr)
		{
			ReceiveThread = FRunnableThread::Create(new FServerReceive(Get()), TEXT("ClientReceive"));
		}
		else
		{
			ReceiveThread->Suspend(false);
		}

		if (SendThread == nullptr)
		{
			SendThread = FRunnableThread::Create(new FServerSend(Get()), TEXT("ClientSend"));
		}
		else
		{
			SendThread->Suspend(false);
		}

		UE_LOG(LogClientNetworkModule, Warning, TEXT("Client connect server success!"));
	}
	else
	{
		OnDisconnection(ERROR_DISCONNECTION);

		UE_LOG(LogClientNetworkModule, Warning, TEXT("Client connect server failed!"));
	}
	
	return Socket;
}

void TAsynTcpClient::Send(const void* Data, int32 DataSize)
{
	FScopeLock* QueueLock = new FScopeLock(&SendCritical);
	SendMessages.Enqueue(MakeShareable(new FDatagram((uint8*)Data, DataSize)));
	delete QueueLock;
}

void TAsynTcpClient::OnDisconnection(ENetworkErrorCode NetworkErrorCode)
{
	bIsConnected = false;

	TSharedPtr<FBase> ReceiveBase = MakeShareable(new FBase(NetworkErrorCode));

	FScopeLock* ReceiveQueueLock = new FScopeLock(&ReceiveCritical);
	ReceiveMessages.Enqueue(ReceiveBase);
	delete ReceiveQueueLock;

	FScopeLock* SendQueueLock = new FScopeLock(&SendCritical);
	SendMessages.Empty();
	delete SendQueueLock;

	if (ReceiveThread != nullptr)
	{
		ReceiveThread->Suspend();
	}

	if (SendThread != nullptr)
	{
		SendThread->Suspend();
	}
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

bool TAsynTcpClient::IsConnected() const
{
	return bIsConnected;
}

TSharedPtr<TAsynTcpClient> TAsynTcpClient::Instance = nullptr;