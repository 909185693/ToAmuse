// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "ClientNetwork.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkFunctionLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectSignature, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginSignature, int32, ErrorCode);

/**
 * 
 */
UCLASS()
class TOAMUSE_API UNetworkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UNetworkFunctionLibrary();
	~UNetworkFunctionLibrary();
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnConnectSignature OnConnect;

	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnLoginSignature OnLogin;

	UFUNCTION(BlueprintCallable, Category = "Network", meta = (WorldContext = "WorldContextObject", DisplayName="GetNetworkFunctionLibrary"))
	static UNetworkFunctionLibrary* Get(UObject* WorldContextObject);

private:
	UFUNCTION(BlueprintCallable, Category = "Network", meta = (WorldContext = "WorldContextObject"))
	static void Connect(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Network|Login", meta = (WorldContext = "WorldContextObject"))
	static void Login(UObject* WorldContextObject, const FText& UserName, const FText& Password);

private:
	static class UNetworkFunctionLibrary* Instance;

protected:
	void OnSocketConnect(bool bWasSuccessful);
};
