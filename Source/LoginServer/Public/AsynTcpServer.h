// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"
#include "Structure.h"

struct FClientMessage
{
	FClientMessage()
	{

	}

	FClientMessage(FSocket* InSocket, const FDatagram& InData)
		: Socket(InSocket)
		, Data(InData)
	{

	}

	FSocket* Socket;

	FDatagram Data;
};

class LOGINSERVER_API TAsynTcpServer
{
public:
	TAsynTcpServer();
	~TAsynTcpServer();

public:
	FSocket* Create();

	void SendClient(FSocket* ClientSocket, const void* InData, int32 InSize);

	void Read(TSharedPtr<FBase>& Data);

	FSocket* Socket;

	static FString Description;
	
	// 客户端列表
	TArray<FSocket*> Clients;

	// 消息队列
	TQueue<TSharedPtr<FClientMessage>> SendMessages;
	TQueue<TSharedPtr<FBase>> ReceiveMessages;

	// 互斥锁
	FCriticalSection ClientsCritical;
	FCriticalSection SendCritical;
	FCriticalSection ReceiveCritical;
	
protected:
	FRunnableThread* AcceptThread;
	FRunnableThread* ReceiveThread;
	FRunnableThread* SendThread;
};