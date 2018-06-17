// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "AsynTcpServer.h"
#include "ClientAccept.h"
#include "ClientReceive.h"
#include "ClientSend.h"


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
			AcceptThread = FRunnableThread::Create(new FClientAccept(this), TEXT("ClientAccept"));
			ReceiveThread = FRunnableThread::Create(new FClientReceive(this), TEXT("ClientReceive"));
			SendThread = FRunnableThread::Create(new FClientSend(this), TEXT("ClientSend"));

			UE_LOG(LogLoginServerModule, Warning, TEXT("Login server start the success!"));
		}
		else
		{
			UE_LOG(LogLoginServerModule, Warning, TEXT("Login server start the failed!"));
		}
	}
	
	return Socket;
}

void TAsynTcpServer::SendClient(FSocket* ClientSocket, const void* InData, int32 InSize)
{
	FScopeLock* SendQueueLock = new FScopeLock(&SendCritical);
	SendMessages.Enqueue(MakeShareable(new FClientMessage(ClientSocket, FDatagram((uint8*)InData, InSize))));
	delete SendQueueLock;
}

void TAsynTcpServer::Read(TSharedPtr<FBase>& Data)
{
	FScopeLock* ReceiveQueueLock = new FScopeLock(&ReceiveCritical);
	if (!ReceiveMessages.IsEmpty())
	{
		ReceiveMessages.Dequeue(Data);
	}
	delete ReceiveQueueLock;
}