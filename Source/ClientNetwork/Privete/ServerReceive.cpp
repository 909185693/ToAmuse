// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ServerReceive.h"


#define MAX_FAILD_READ_COUNT 3
#define LIMIT_READER_SIZE 65536u // 数据大小限制

FServerReceive::FServerReceive(TSharedPtr<TAsynTcpClient, ESPMode::ThreadSafe> InAsynTcpClient)
	: AsynTcpClient(InAsynTcpClient)
	, bStopping(false)
	, FailedReadCount(0)
{

}

FServerReceive::~FServerReceive()
{
	AsynTcpClient.Reset();
}

bool FServerReceive::Init()
{
	bStopping = false;

	return true;
}

uint32 FServerReceive::Run()
{
	if (!AsynTcpClient.IsValid())
	{
		return 0;
	}

	TArray<uint8> ReceiveData;
	
	ReceiveData.Init(0.f, 65507u);

	const FTimespan WaitTime(200);
	
	while (!bStopping)
	{
		FSocket*& Socket = AsynTcpClient->Socket;
		if (Socket != nullptr)
		{
			if (!Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
			{
				continue;
			}

			FArrayReaderPtr Reader = MakeShareable(new FArrayReader(true));
			Reader->SetNumUninitialized(LIMIT_READER_SIZE);
			
			int32 Read = 0;
			Socket->Recv(Reader->GetData(), LIMIT_READER_SIZE, Read);
			if (0 < Read)
			{
				FailedReadCount = 0;

				Reader->SetNumUninitialized(Read);

				AsynTcpClient->ReceiveMessages.Enqueue(MakeShareable((FBase*)Reader->GetData()));
			}
			else
			{
				FailedReadCount++;

				if (FailedReadCount >= 3)
				{
					FailedReadCount = 0;

					AsynTcpClient->OnSocketBreaked();
				}
			}
		}
	}

	return 0;
}

void FServerReceive::Stop()
{
	bStopping = true;
}

void FServerReceive::Exit()
{
	
}