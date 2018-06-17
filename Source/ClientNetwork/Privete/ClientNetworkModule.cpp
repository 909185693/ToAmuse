#include "ClientNetwork.h"
#include "ClientNetworkModule.h"
#include "ModuleManager.h"


DEFINE_LOG_CATEGORY(LogClientNetworkModule)

void FClientNetworkModule::StartupModule()
{
	UE_LOG(LogClientNetworkModule, Warning, TEXT("StartupModule Complete"));
}

void FClientNetworkModule::ShutdownModule()
{
	UE_LOG(LogClientNetworkModule, Warning, TEXT("ShutdownModule"));
}

bool FClientNetworkModule::IsGameModule() const
{
	return false;
}

IMPLEMENT_MODULE(FClientNetworkModule, ClientNetwork);