// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Engine/DataTable.h" 
#include "DataAsset/VDTableRegistry.h"
#include "VDDataTableSubSystem.generated.h"

UCLASS()
class PROJECTVD_API UVDDataTableSubSystem : public USubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSoftObjectPtr<UVDTableRegistry> TableRegistry;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UVDDataTableSubSystem();

	template<typename T>
	UDataTable* GetDataTable(const FName& InTableKey)
	{
		if (!TableRegistry.IsValid())
		{
			TableRegistry.LoadSynchronous();
		}

		if (TableRegistry.IsValid())
		{
			TSoftObjectPtr<UDataTable> DataTablePtr = TableRegistry->GetDataTable<T>(InTableKey);
			if (DataTablePtr.IsValid())
			{
				return DataTablePtr.Get();
			}
			else if (!DataTablePtr.IsNull())
			{
				return DataTablePtr.LoadSynchronous();
			}
		}

		return nullptr;
	}

	template<typename T>
	T* GetDataTableRow(const FName& InTableKey, const FName& InRowKey)
	{
		UDataTable* DataTable = GetDataTable<T>(InTableKey);
		if (DataTable)
		{
			return DataTable->FindRow<T>(InRowKey, TEXT(""));
		}

		return nullptr;
	}
};
