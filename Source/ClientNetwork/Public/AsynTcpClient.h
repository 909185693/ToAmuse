// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"
#include "Structure.h"

class FServerReceive;
class FConnectServer;

class CLIENTNETWORK_API TAsynTcpClient
{
public:
	TAsynTcpClient();
	~TAsynTcpClient();

public:
	FSocket* Connect(const FString& IP, int32 Port);

	void Send(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& Data);
		
	bool Read(TSharedPtr<FBase, ESPMode::ThreadSafe>& OutData);

	static TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> Get();

	// Socket
	FSocket* Socket;

	// socket描述
	static FString Description;
	
	// 客户端列表
	TArray<FSocket*> Clients;

	// 接收消息队列
	TQueue<TSharedPtr<FBase, ESPMode::ThreadSafe>, EQueueMode::Mpsc> ReceiveMessages;

protected:
	FRunnableThread* ReceiveThread;
	FRunnableThread* ConnectThread;

protected:
	DECLARE_DELEGATE_OneParam(FOnConnect, bool /*bWasSuccessful*/);
	FOnConnect OnConnectDelegate;

	DECLARE_DELEGATE(FOnBreaked);
	FOnBreaked OnBreakedDelegate;

public:
	FOnConnect& OnConnect() { return OnConnectDelegate; }
	FOnBreaked& OnBreaked() { return OnBreakedDelegate; }

	void OnSocketConnect(bool bWasSuccessful);
	void OnSocketBreaked();
	
private:
	static TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> Instance;
};