// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "AsynTcpServer.h"


class FClientReceive : public FRunnable
{
public:
	FClientReceive(TAsynTcpServer* InAsynTcpServer);
	~FClientReceive();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	TAsynTcpServer* AsynTcpServer;
};