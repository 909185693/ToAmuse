// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "AsynTcpServer.h"


TAsynTcpServer::TAsynTcpServer()
{

}

TAsynTcpServer::~TAsynTcpServer()
{

}

FSocket* TAsynTcpServer::Create()
{
	if (Socket == nullptr)
	{
		Socket = FTcpSocketBuilder(TEXT("FAsynTcpServer"))
			.AsNonBlocking()
			.Listening(0);

		TSharedRef<FInternetAddr> LocalAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		if (Socket->Bind(LocalAddress.Get()))
		{
			UE_LOG(LogLoginServerModule, Warning, TEXT("Login server start the success!"));
		}
		else
		{
			UE_LOG(LogLoginServerModule, Warning, TEXT("Login server start the failed!"));
		}
	}

	//Thread = FRunnableThread::Create(this, TEXT("FAsynTcpServer"), THREAD_STACK_SIZE, TPri_Normal);

	return Socket;
}