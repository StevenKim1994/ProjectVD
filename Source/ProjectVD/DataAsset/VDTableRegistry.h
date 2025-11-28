// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VDTableRegistry.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDTableRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	template<typename T>
	TSoftObjectPtr<UDataTable> GetDataTable(const FName& InKey) const
	{
		if (const TSoftObjectPtr<UDataTable>* FoundTable = DataTableMap.Find(InKey))
		{
			return *FoundTable;
		}
		return nullptr;
	}

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FName(TEXT("Table")), GetFName());
	}

	const TMap<FName, TSoftObjectPtr<UDataTable>>& GetDataTableMap() const
	{
		return DataTableMap;
	}

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
	TMap<FName, TSoftObjectPtr<UDataTable>> DataTableMap;
	
};
