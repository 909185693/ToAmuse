// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"


class FClientSend : public FRunnable
{
public:
	FClientSend(FSocket* InSocket);
	~FClientSend();

public:
	virtual void Send();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	FSocket* Socket;
};