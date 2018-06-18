// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "AsynTcpServer.h"


class FLogicProcess : public FRunnable
{
public:
	FLogicProcess(TSharedPtr<TAsynTcpServer> InAsynTcpServer);
	~FLogicProcess();

public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

protected:
	void Process(FSocket* Socket, const TSharedPtr<FBase>& Data);
	void OnLogin(FSocket* Socket, FLoginInfo LoginInfo);

private:
	bool bStopping;

	TSharedPtr<TAsynTcpServer> AsynTcpServer;
};