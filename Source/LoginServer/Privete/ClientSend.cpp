// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "ClientSend.h"


FClientSend::FClientSend(TSharedPtr<TAsynTcpServer> InAsynTcpServer)
	: AsynTcpServer(InAsynTcpServer)
{

}

FClientSend::~FClientSend()
{
	AsynTcpServer.Reset();
}

bool FClientSend::Init()
{
	bStopping = false;

	return true;
}

uint32 FClientSend::Run()
{
	if (!AsynTcpServer.IsValid())
	{
		return 0;
	}

	while (!bStopping)
	{
		FScopeLock* SendQueueLock = new FScopeLock(&AsynTcpServer->SendCritical);
		if (!AsynTcpServer->SendMessages.IsEmpty())
		{
			TSharedPtr<FSendMessage> Message;
			AsynTcpServer->SendMessages.Dequeue(Message);

			if (Message.IsValid())
			{
				if (FSocket* Socket = Message->Socket)
				{
					int32 Sent = 0;

					if (Socket->Send(Message->Data.Data, Message->Data.Size, Sent))
					{
						UE_LOG(LogLoginServerModule, Warning, TEXT("FClientSend::Run() Success! Code[%d] Size[%d]"), ((FBase*)Message->Data.Data)->Code, Message->Data.Size);
					}
					else
					{
						UE_LOG(LogLoginServerModule, Warning, TEXT("FClientSend::Run() Failed!"));
					}
				}

				Message.Reset();
			}
		}
		delete SendQueueLock;
	}

	return 0;
}

void FClientSend::Stop()
{
	bStopping = true;
}

void FClientSend::Exit()
{

}