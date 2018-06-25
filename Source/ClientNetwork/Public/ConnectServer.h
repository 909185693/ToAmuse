// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "AsynTcpClient.h"


class FConnectServer : public FRunnable
{
public:
	FConnectServer(TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> InAsynTcpClient, const FString& InIP, int32 InPort);
	~FConnectServer();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	bool bHostConnected;

	int32 ConnectedFailedNum;

	FString IP;

	int32 Port;

	TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> AsynTcpClient;
};