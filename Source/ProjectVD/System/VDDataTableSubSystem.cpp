// Fill out your copyright notice in the Description page of Project Settings.

#include "System/VDDataTableSubSystem.h"
#include "Engine/DataTable.h"

void UVDDataTableSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDDataTableSubSystem::Deinitialize()
{
	RegisteredDataTables.Empty();
	LoadedDataTables.Empty();
	Super::Deinitialize();
}

void UVDDataTableSubSystem::RegisterTable(FName TableName, UDataTable* DataTable)
{
	if (!TableName.IsNone() && DataTable)
	{
		RegisteredDataTables.Add(TableName, DataTable);
		LoadedDataTables.Remove(TableName);
	}
}
