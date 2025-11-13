// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDResourceSystem.h"
#include "DataAsset/VDMovieRegistry.h"
#include "DataAsset/VDUIRegistry.h"

void UVDResourceSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	AssetManager = UAssetManager::GetIfInitialized();

	FPrimaryAssetId Id("UI","UIRegistry");
	LoadPrimaryAsset(Id);

	UE_LOG(LogTemp, Warning, TEXT("UVDResourceSystem Initialize"));
}

void UVDResourceSystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("UVDResourceSystem Deinitialize"));
}

void UVDResourceSystem::LoadPrimaryAssetAsync(const FPrimaryAssetId& AssetId, TFunction<void(UPrimaryDataAsset*)> OnLoadedCallback)
{
	if (AssetId.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		AssetManager->LoadPrimaryAsset(AssetId, TArray<FName>(),
			FStreamableDelegate::CreateLambda([this, AssetId, OnLoadedCallback]()
				{
					UPrimaryDataAsset* LoadedAsset = AssetManager->GetPrimaryAssetObject<UPrimaryDataAsset>(AssetId);
					if (LoadedAsset)
					{
						if (LoadedPrimaryAssets.Contains(AssetId) == false)
						{
							LoadedPrimaryAssets.Add(AssetId, LoadedAsset);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Primary Asset %s is already loaded."), *AssetId.ToString());
						}
					}
					OnLoadedCallback(LoadedAsset);
				})
		);
	}
}

void UVDResourceSystem::LoadPrimaryAsset(const FPrimaryAssetId& AssetId)
{
	//if (AssetId.IsValid())
	{
		UPrimaryDataAsset* LoadedAsset = AssetManager->GetPrimaryAssetObject<UPrimaryDataAsset >(AssetId);
		if (LoadedAsset)
		{
			if (LoadedPrimaryAssets.Contains(AssetId) == false)
			{
				LoadedPrimaryAssets.Add(AssetId, LoadedAsset);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Primary Asset %s is already loaded."), *AssetId.ToString());
			}
		}
	}
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

void UVDResourceSystem::LoadResourceAsync(const FSoftObjectPath& ResourcePath, TFunction<void(UObject*)> OnLoadedCallback)
{
	if(ResourcePath.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.RequestAsyncLoad(ResourcePath,
			FStreamableDelegate::CreateLambda([ResourcePath, OnLoadedCallback]()
				{
					UObject* LoadedObject = ResourcePath.ResolveObject();
					OnLoadedCallback(LoadedObject);
				})
		);
	}
}







