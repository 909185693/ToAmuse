// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"


class FClientReceive : public FRunnable
{
public:
	FClientReceive(FSocket* InSocket);
	~FClientReceive();

public:
	virtual void Receive();

public:

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bStopping;

	FSocket* Socket;
};