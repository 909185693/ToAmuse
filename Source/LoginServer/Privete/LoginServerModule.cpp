#include "LoginServer.h"
#include "LoginServerModule.h"
#include "ModuleManager.h"


DEFINE_LOG_CATEGORY(LogLoginServerModule)

void FLoginServerModule::StartupModule()
{
	UE_LOG(LogLoginServerModule, Warning, TEXT("StartupModule Complete"));
}

void FLoginServerModule::ShutdownModule()
{
	UE_LOG(LogLoginServerModule, Warning, TEXT("ShutdownModule"));
}

bool FLoginServerModule::IsGameModule() const
{
	return false;
}

void FLoginServerModule::Create()
{

}

IMPLEMENT_MODULE(FLoginServerModule, LoginServer);