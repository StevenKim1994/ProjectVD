// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDResourceSystem.h"

void UVDResourceSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	AssetManager = UAssetManager::Get();

	UE_LOG(LogTemp, Warning, TEXT("UVDResourceSystem Initialize"));
}

void UVDResourceSystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("UVDResourceSystem Deinitialize"));
}

void UVDResourceSystem::LoadResourceAsync(const TSoftObjectPtr<UObject>& ResourcePtr, TFunction<void(UObject*)> OnLoadedCallback)
{
	if (ResourcePtr.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.RequestAsyncLoad(ResourcePtr.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([ResourcePtr, OnLoadedCallback]()
				{
					UObject* LoadedObject = ResourcePtr.Get();
					OnLoadedCallback(LoadedObject);
				})
		);
	}
}

void UVDResourceSystem::LoadResourceAsync(const FSoftClassPath& ResourcePath, TFunction<void(UClass*)> OnLoadedCallback)
{
	if(ResourcePath.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.RequestAsyncLoad(ResourcePath,
			FStreamableDelegate::CreateLambda([ResourcePath, OnLoadedCallback]()
				{
					UClass* LoadedClass = ResourcePath.ResolveClass();
					OnLoadedCallback(LoadedClass);
				})
		);
	}
}
