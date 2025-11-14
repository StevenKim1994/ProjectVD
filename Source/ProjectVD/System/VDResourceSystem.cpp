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

void UVDResourceSystem::LoadPrimaryAssetAsync(const FPrimaryAssetId& AssetId, FOnPrimaryAssetLoaded OnLoadedCallback)
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

					if (OnLoadedCallback.IsBound())
					{
						OnLoadedCallback.Execute(LoadedAsset);
					}
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


void UVDResourceSystem::LoadResourceAsync(const TSoftObjectPtr<UObject>& ResourcePtr, FOnUIWidgetLoaded OnLoadedCallback)
{
	if (ResourcePtr.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.RequestAsyncLoad(ResourcePtr.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([ResourcePtr, OnLoadedCallback]()
				{
					UObject* LoadedObject = ResourcePtr.Get();

					if (OnLoadedCallback.IsBound())
					{
						OnLoadedCallback.Execute(LoadedObject);
					}
				})
		);
	}
}

void UVDResourceSystem::LoadResourceAsync(const FSoftClassPath& ResourcePath, FOnClassLoaded OnLoadedCallback)
{
	if(ResourcePath.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.RequestAsyncLoad(ResourcePath,
			FStreamableDelegate::CreateLambda([ResourcePath, OnLoadedCallback]()
				{
					UClass* LoadedClass = ResourcePath.ResolveClass();

					if (OnLoadedCallback.IsBound())
					{
						OnLoadedCallback.Execute(LoadedClass);
					}
				})
		);
	}
}

UClass* UVDResourceSystem::LoadResource(const FSoftClassPath& ResourcePath)
{
	if (AssetManager)
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.LoadSynchronous(ResourcePath, true /*bManageActiveHandle*/);
		return ResourcePath.ResolveClass();
	}

	return nullptr;
}

void UVDResourceSystem::LoadResourceAsync(const FSoftObjectPath& ResourcePath, FOnUIWidgetLoaded OnLoadedCallback)
{
	if(ResourcePath.IsValid())
	{
		FStreamableManager& Streamable = AssetManager->GetStreamableManager();
		Streamable.RequestAsyncLoad(ResourcePath,
			FStreamableDelegate::CreateLambda([ResourcePath, OnLoadedCallback]()
				{
					UObject* LoadedObject = ResourcePath.ResolveObject();

					if (OnLoadedCallback.IsBound())
					{
						OnLoadedCallback.Execute(LoadedObject);
					}
				})
		);
	}
}







