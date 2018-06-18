// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "LoginServer.h"
#include "LoginServerModule.h"
#include "LogicProcess.h"
#include "Macro.h"


FLogicProcess::FLogicProcess(TSharedPtr<TAsynTcpServer> InAsynTcpServer)
	: AsynTcpServer(InAsynTcpServer)
{

}

FLogicProcess::~FLogicProcess()
{
	AsynTcpServer.Reset();
}

bool FLogicProcess::Init()
{
	bStopping = false;

	return true;
}

uint32 FLogicProcess::Run()
{
	if (!AsynTcpServer.IsValid())
	{
		return 0;
	}

	while (!bStopping)
	{
		TSharedPtr<FReceiveMessage> ReceiveMessage;
		TAsynTcpServer::Get()->Read(ReceiveMessage);
		if (ReceiveMessage.IsValid())
		{
			Process(ReceiveMessage->Socket, ReceiveMessage->Data);
		}
		ReceiveMessage.Reset();
	}

	return 0;
}

void FLogicProcess::Stop()
{
	bStopping = true;
}

void FLogicProcess::Exit()
{

}

void FLogicProcess::Process(FSocket* Socket, const TSharedPtr<FBase>& Data)
{
	if (Socket == nullptr)
	{
		return;
	}

	if (Data.IsValid())
	{
		switch (Data->Code)
		{
		case USER_LOGIN:
			OnLogin(Socket, *(FLoginInfo*)Data.Get());
			break;
		default:
			break;
		}
	}

	UE_LOG(LogLoginServerModule, Warning, TEXT("FLogicProcess::Process() Code[%d]"), Data->Code);
}

void FLogicProcess::OnLogin(FSocket* Socket, FLoginInfo LoginInfo)
{
	FBase Reply(USER_LOGIN, ERROR_LOGIN_FAILED);
	if (FCString::Strlen(LoginInfo.Password) >= 6 && FCString::Strlen(LoginInfo.Password) > 6)
	{
		Reply.Error = ERROR_NONE;

		//uint32 ProcessId = FPlatformProcess::GetCurrentProcessId();
		//FString ApplicationName = FPlatformProcess::GetApplicationName(ProcessId);

		//UE_LOG(LogLoginServerModule, Warning, TEXT("ApplicationName [%s]"), *ApplicationName);

		//FPlatformProcess::CreateProc(TEXT(""))
	}

	TAsynTcpServer::Get()->SendClient(Socket, &Reply, sizeof(FBase));

}