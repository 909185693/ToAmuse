// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "AsynTcpClient.h"


class FServerReceive : public FRunnable
{
public:
	FServerReceive(TSharedPtr<TAsynTcpClient> InAsynTcpClient);
	~FServerReceive();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	TSharedPtr<TAsynTcpClient> AsynTcpClient;
};