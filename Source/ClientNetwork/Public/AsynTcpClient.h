// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"
#include "Structure.h"


class CLIENTNETWORK_API TAsynTcpClient
{
public:
	TAsynTcpClient();
	~TAsynTcpClient();

public:
	FSocket* Connect(const FString& IP, int32 Port);

	void Send(const void* Data, int32 DataSize);

	void Read(TSharedPtr<FBase>& Data);

	static TSharedPtr<TAsynTcpClient> Get();

	FSocket* Socket;

	static FString Description;
	
	// 客户端列表
	TArray<FSocket*> Clients;

	// 消息队列
	TQueue<TSharedPtr<FDatagram>> SendMessages;
	TQueue<TSharedPtr<FBase>> ReceiveMessages;

	// 互斥锁
	FCriticalSection SocketCritical;
	FCriticalSection SendCritical;
	FCriticalSection ReceiveCritical;
	
protected:
	FRunnableThread* ReceiveThread;
	FRunnableThread* SendThread;

private:
	static TSharedPtr<TAsynTcpClient> Instance;
};