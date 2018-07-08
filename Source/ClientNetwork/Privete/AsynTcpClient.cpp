// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "AsynTcpClient.h"
#include "ConnectServer.h"
#include "ServerReceive.h"


FString TAsynTcpClient::Description = TEXT("AsynTcpServer");

TAsynTcpClient::TAsynTcpClient()
	: Socket(nullptr)
	, ReceiveThread(nullptr)
	, ConnectThread(nullptr)
{

}

TAsynTcpClient::~TAsynTcpClient()
{

}

TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> TAsynTcpClient::Get()
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
		Socket = FTcpSocketBuilder(Description).AsNonBlocking();
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

void TAsynTcpClient::Send(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& Data)
{
	int32 Sent = 0;

	Socket->Send(Data->GetData(), Data->Num(), Sent);
}

bool TAsynTcpClient::Read(TSharedPtr<FBase, ESPMode::ThreadSafe>& OutData)
{
	if (ReceiveMessages.Dequeue(OutData))
	{
		return true;
	}

	return false;
}

void TAsynTcpClient::OnSocketConnect(bool bWasSuccessful)
{
	AsyncTask(ENamedThreads::GameThread, [&, bWasSuccessful] {
		
		if (ConnectThread != nullptr)
		{
			ConnectThread->Suspend(true);
		}

		if (bWasSuccessful)
		{
			if (ReceiveThread == nullptr)
			{
				ReceiveThread = FRunnableThread::Create(new FServerReceive(Get()), TEXT("ServerReceive"));
			}
			else
			{
				ReceiveThread->Suspend(false);
			}			
		}
		else
		{
			if (ReceiveThread != nullptr)
			{
				ReceiveThread->Suspend(true);
			}
		}

		OnConnect().ExecuteIfBound(bWasSuccessful);
	
	});

	UE_LOG(LogClientNetworkModule, Warning, TEXT("void TAsynTcpClient::OnSocketConnect(bWasSuccessful [%d])"), bWasSuccessful);
}

void TAsynTcpClient::OnSocketBreaked()
{
	OnSocketConnect(false);
}

TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> TAsynTcpClient::Instance = nullptr;