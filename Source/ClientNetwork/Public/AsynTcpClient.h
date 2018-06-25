// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"
#include "Structure.h"

#define THREAD_STACK_SIZE 1 * 1024 * 1024

class CLIENTNETWORK_API TAsynTcpClient
{
public:
	TAsynTcpClient();
	~TAsynTcpClient();

public:
	TSharedPtr<FSocket, ESPMode::ThreadSafe> Connect(const FString& IP, int32 Port);

	void OnConnect();

	void OnDisconnection(ENetworkErrorCode NetworkErrorCode);

	void Send(const void* Data, int32 DataSize);

	void Read(TSharedPtr<FBase, ESPMode::ThreadSafe>& Data);
	
	static TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> Get();

	TSharedPtr<FSocket, ESPMode::ThreadSafe> Socket;

	static FString Description;
	
	// 客户端列表
	TArray<FSocket*> Clients;

	// 消息队列
	TQueue<TSharedPtr<FDatagram, ESPMode::ThreadSafe>, EQueueMode::Mpsc> SendMessages;
	TQueue<TSharedPtr<FBase, ESPMode::ThreadSafe>, EQueueMode::Mpsc> ReceiveMessages;

	// 互斥锁
	FCriticalSection Mutex;
	
protected:
	FRunnableThread* ReceiveThread;
	FRunnableThread* SendThread;
	FRunnableThread* ConnectThread;
	
private:
	static TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> Instance;
};