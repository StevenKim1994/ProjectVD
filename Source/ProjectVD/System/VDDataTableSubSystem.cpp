// Fill out your copyright notice in the Description page of Project Settings.

#include "System/VDDataTableSubSystem.h"
#include "Engine/DataTable.h"
#include "Public/VDConstrants.h"

void UVDDataTableSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDDataTableSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

UVDDataTableSubSystem::UVDDataTableSubSystem()
{
	ConstructorHelpers::FObjectFinder<UVDTableRegistry> TableRegistryObj(*VDConstants::TableRegistryDataAssetPath);
	if (TableRegistryObj.Succeeded())
	{
		TableRegistry = TableRegistryObj.Object;
	}
}
