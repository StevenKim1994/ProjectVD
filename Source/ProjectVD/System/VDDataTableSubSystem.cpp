// Fill out your copyright notice in the Description page of Project Settings.

#include "System/VDDataTableSubSystem.h"
#include "Engine/DataTable.h"
#include "Public/VDConstrants.h"
#include "System/VDResourceSystem.h"

void UVDDataTableSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UVDResourceSystem* ResourceSystem = Collection.InitializeDependency<UVDResourceSystem>();
	//TableRegistry = ResourceSystem->GetLoadedPrimaryAsset<UVDTableRegistry>(FPrimaryAssetId(FName(TEXT("Table")), FName(TEXT("PAL_TableRegistry"))));
}

void UVDDataTableSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

UVDDataTableSubSystem::UVDDataTableSubSystem()
{
	TableRegistry = ConstructorHelpers::FObjectFinder<UVDTableRegistry>(TEXT("/Game/ProjectVD/DataAsset/PAL_TableRegistry.PAL_TableRegistry")).Object;
}
