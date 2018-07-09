// Copyright 2018 by January. All Rights Reserved.

#include "TGameplayStatics.h"


UTGameplayStatics::UTGameplayStatics(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UDataTable* UTGameplayStatics::GetDataTable(FString DataTableAssetPath)
{
	return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTableAssetPath, nullptr, LOAD_None, nullptr));
}

template<typename T>
T* UTGameplayStatics::GetDataRow(FString DataTableAssetPath, int32 RowID)
{
	UDataTable* DataTable = GetDataTable(DataTableAssetPath);
	if (DataTable != nullptr)
	{
		for (TMap<FName, uint8*>::TConstIterator RowMapIter(DataTable->RowMap.CreateConstIterator()); RowMapIter; ++RowMapIter)
		{
			T* DataRow = reinterpret_cast<T*>(RowMapIter.Value());
			if (DataRow && DataRow->ID == RowID)
			{
				return DataRow;
			}
		}
	}

	return nullptr;
}