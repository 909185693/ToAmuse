// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "AsynTcpClient.h"


class FServerSend : public FRunnable
{
public:
	FServerSend(TSharedPtr<TAsynTcpClient> InAsynTcpClient);
	~FServerSend();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	TSharedPtr<TAsynTcpClient> AsynTcpClient;
};