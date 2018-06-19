// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "AsynTcpServer.h"
#include "ClientAccept.h"
#include "ClientReceive.h"
#include "ClientSend.h"
#include "LogicProcess.h"


FString TAsynTcpServer::Description = TEXT("AsynTcpServer");

TAsynTcpServer::TAsynTcpServer()
	: Socket(nullptr)
{

}

TAsynTcpServer::~TAsynTcpServer()
{
	if (Socket != nullptr)
	{
		Socket->Close();
		Socket = nullptr;
	}
}

TSharedPtr<TAsynTcpServer> TAsynTcpServer::Get()
{
	if (!Instance.IsValid())
	{
		Instance = MakeShareable(new TAsynTcpServer());
	}

	return Instance;
}

FSocket* TAsynTcpServer::Create()
{
	if (Socket == nullptr)
	{
		Socket = FTcpSocketBuilder(*Description)
			.BoundToPort(8000)
			.AsNonBlocking()
			.Listening(0);
		
		if (Socket != nullptr)
		{
			AcceptThread = FRunnableThread::Create(new FClientAccept(Get()), TEXT("ClientAccept"));
			ReceiveThread = FRunnableThread::Create(new FClientReceive(Get()), TEXT("ClientReceive"));
			SendThread = FRunnableThread::Create(new FClientSend(Get()), TEXT("ClientSend"));
			LogicThread = FRunnableThread::Create(new FLogicProcess(Get()), TEXT("LogicProcess"));

			UE_LOG(LogLoginServerModule, Warning, TEXT("Login server start the success!"));
		}
		else
		{
			UE_LOG(LogLoginServerModule, Warning, TEXT("Login server start the failed!"));
		}
	}
	
	return Socket;
}

void TAsynTcpServer::SendClient(FSocket* ClientSocket, void* InData, int32 InSize)
{
	FScopeLock* SendQueueLock = new FScopeLock(&SendCritical);
	TSharedPtr<FDatagram> Datagram = MakeShareable(new FDatagram((uint8*)InData, InSize));
	TSharedPtr<FSendMessage> SendMessage = MakeShareable(new FSendMessage(ClientSocket, Datagram));
	SendMessages.Enqueue(SendMessage);
	delete SendQueueLock;
}

void TAsynTcpServer::Read(TSharedPtr<FReceiveMessage>& Data)
{
	FScopeLock* ReceiveQueueLock = new FScopeLock(&ReceiveCritical);
	if (!ReceiveMessages.IsEmpty())
	{
		ReceiveMessages.Dequeue(Data);
	}
	delete ReceiveQueueLock;
}

TSharedPtr<TAsynTcpServer> TAsynTcpServer::Instance = nullptr;