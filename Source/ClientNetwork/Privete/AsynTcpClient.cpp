// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "AsynTcpClient.h"
#include "ConnectServer.h"
#include "ServerReceive.h"
#include "ServerSend.h"


FString TAsynTcpClient::Description = TEXT("AsynTcpServer");

TAsynTcpClient::TAsynTcpClient()
	: Socket(nullptr)
	, ReceiveThread(nullptr)
	, SendThread(nullptr)
	, ConnectThread(nullptr)
{

}

TAsynTcpClient::~TAsynTcpClient()
{
	if (Socket.IsValid())
	{
		Socket->Close();
		Socket = nullptr;
	}

	if (ConnectThread != nullptr)
	{
		ConnectThread->Kill(true);
		delete ConnectThread;
		ConnectThread = nullptr;
	}

	if (ReceiveThread != nullptr)
	{
		ReceiveThread->Kill(true);
		delete ReceiveThread;
		ReceiveThread = nullptr;
	}

	if (SendThread != nullptr)
	{
		SendThread->Kill(true);
		delete SendThread;
		SendThread = nullptr;
	}
}

TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> TAsynTcpClient::Get()
{
	if (!Instance.IsValid())
	{
		Instance = MakeShareable(new TAsynTcpClient());
	}

	return Instance;
}

TSharedPtr<FSocket, ESPMode::ThreadSafe> TAsynTcpClient::Connect(const FString& IP, int32 Port)
{
	if (!Socket.IsValid())
	{
		Socket = MakeShareable(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, Description, false));
		Socket->SetNonBlocking();
	}

	FIPv4Address IPv4Address;
	FIPv4Address::Parse(IP, IPv4Address);

	TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(IPv4Address.Value);
	InternetAddr->SetPort(Port);

	Socket->Connect(*InternetAddr);

	if (ConnectThread == nullptr)
	{
		ConnectThread = FRunnableThread::Create(new FConnectServer(Get(), IP, Port), TEXT("ConnectServer"));
	}
	else
	{
		ConnectThread->Suspend(false);
	}
	
	
	return Socket;
}

void TAsynTcpClient::OnConnect()
{
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

	if (ConnectThread != nullptr)
	{
		ConnectThread->Suspend(true);
	}

	UE_LOG(LogClientNetworkModule, Warning, TEXT("Client connect server success!"));
}

void TAsynTcpClient::OnDisconnection(ENetworkErrorCode NetworkErrorCode)
{
	TSharedPtr<FBase, ESPMode::ThreadSafe> ReceiveBase = MakeShareable(new FBase(NetworkErrorCode));
	ReceiveMessages.Enqueue(ReceiveBase);

	SendMessages.Empty();

	if (Socket.IsValid())
	{
		//FSocket* SocketPtr = Socket.Get();

		//Socket->Close();
		//Socket.Reset();
		//Socket = nullptr;

		//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketPtr);
	}

	UE_LOG(LogClientNetworkModule, Warning, TEXT("Client connect server error!()"));
}

void TAsynTcpClient::Send(const void* Data, int32 DataSize)
{
	SendMessages.Enqueue(MakeShareable(new FDatagram((uint8*)Data, DataSize)));
}

void TAsynTcpClient::Read(TSharedPtr<FBase, ESPMode::ThreadSafe>& Data)
{
	if (!ReceiveMessages.IsEmpty())
	{
		ReceiveMessages.Dequeue(Data);
	}
}

TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> TAsynTcpClient::Instance = nullptr;