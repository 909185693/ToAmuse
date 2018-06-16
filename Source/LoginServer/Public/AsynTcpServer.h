// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Sockets.h"
#include "Networking.h"


class TAsynTcpServer
{
public:
	TAsynTcpServer();
	~TAsynTcpServer();

public:
	FSocket* Create();

	FSocket* Socket;
};