// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"
#include "Structure.h"

struct FSendMessage
{
	FSendMessage()
	{

	}

	FSendMessage(FSocket* InSocket, TSharedPtr<FDatagram>& InData)
		: Socket(InSocket)
		, Data(InData)
	{

	}

	~FSendMessage()
	{
		Data.Reset();
	}

	FSocket* Socket;

	TSharedPtr<FDatagram> Data;
};

struct FReceiveMessage
{
	FReceiveMessage(FSocket* InSocket, TSharedPtr<FBase>& InData)
		: Socket(InSocket)
		, Data(InData)
	{

	}

	~FReceiveMessage()
	{
		Data.Reset();
	}

	FSocket* Socket;

	TSharedPtr<FBase> Data;
};

class LOGINSERVER_API TAsynTcpServer
{
public:
	TAsynTcpServer();
	~TAsynTcpServer();

public:
	FSocket* Create();

	void SendClient(FSocket* ClientSocket, void* InData, int32 InSize);

	void Read(TSharedPtr<FReceiveMessage>& Data);

	static TSharedPtr<TAsynTcpServer> Get();

	FSocket* Socket;

	static FString Description;
	
	// 客户端列表
	TArray<FSocket*> Clients;

	// 消息队列
	TQueue<TSharedPtr<FSendMessage>> SendMessages;
	TQueue<TSharedPtr<FReceiveMessage>> ReceiveMessages;

	// 互斥锁
	FCriticalSection ClientsCritical;
	FCriticalSection SendCritical;
	FCriticalSection ReceiveCritical;
	
protected:
	FRunnableThread* AcceptThread;
	FRunnableThread* ReceiveThread;
	FRunnableThread* SendThread;
	FRunnableThread* LogicThread;

private:
	static TSharedPtr<TAsynTcpServer> Instance;
};