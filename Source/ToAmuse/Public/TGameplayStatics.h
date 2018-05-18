// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Object.h"
#include "Kismet/GameplayStatics.h"
#include "TGameplayStatics.generated.h"


/**
* UTGameplayStatics
*/
UCLASS()
class TOAMUSE_API UTGameplayStatics : public UGameplayStatics
{
	GENERATED_UCLASS_BODY()
		
private:
	static UDataTable* GetDataTable(FString DataTableAssetPath);

	template<typename T>
	static T* GetDataRow(FString DataTableAssetPath, int32 RowID);
	
};
