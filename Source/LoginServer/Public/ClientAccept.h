// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "AsynTcpServer.h"


class FClientAccept : public FRunnable
{
public:
	FClientAccept(TAsynTcpServer* InAsynTcpServer);
	~FClientAccept();

public:
	virtual void Accept();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	TAsynTcpServer* AsynTcpServer;	
};