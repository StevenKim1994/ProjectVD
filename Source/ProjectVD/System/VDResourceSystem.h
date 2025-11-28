// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/DataAsset.h"
#include "VDResourceSystem.generated.h"

DECLARE_DELEGATE_OneParam(FOnClassLoaded, UClass*);
DECLARE_DELEGATE_OneParam(FOnUIWidgetLoaded, UObject*);
DECLARE_DELEGATE_OneParam(FOnPrimaryAssetLoaded, UPrimaryDataAsset*);

UCLASS()
class PROJECTVD_API UVDResourceSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UPrimaryDataAsset>> LoadedPrimaryAssets;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void LoadResourceAsync(const TSoftObjectPtr<UObject>& ResourcePtr, FOnUIWidgetLoaded OnLoadedCallback);
	void LoadResourceAsync(const FSoftClassPath& ResourcePath, FOnClassLoaded OnLoadedCallback);
	UClass* LoadResource(const FSoftClassPath& ResourcePath);
	void LoadResourceAsync(const FSoftObjectPath& ResourcePath, FOnUIWidgetLoaded OnLoadedCallback);
	void LoadPrimaryAssetAsync(const FPrimaryAssetId& AssetId, FOnPrimaryAssetLoaded OnLoadedCallback);
	void LoadPrimaryAsset(const FPrimaryAssetId& AssetId);

	template<typename T>
	T* GetLoadedPrimaryAsset(const FPrimaryAssetId& AssetId)
	{
		static_assert(TIsDerivedFrom<T, UPrimaryDataAsset>::IsDerived, "T must derive from UPrimaryDataAsset");

		if (LoadedPrimaryAssets.Contains(AssetId))
		{
			return Cast<T>(LoadedPrimaryAssets[AssetId]);
		}

		const FSoftObjectPath AssetPath = AssetManager->GetPrimaryAssetPath(AssetId);
		UObject* LoadObject = AssetManager->GetStreamableManager().LoadSynchronous(AssetPath, true /*bManageActiveHandle*/);

		if (LoadObject)
		{
			if (UPrimaryDataAsset* PrimaryAsset = Cast<UPrimaryDataAsset>(LoadObject))
			{
				LoadedPrimaryAssets.Add(AssetId, PrimaryAsset);
				return Cast<T>(PrimaryAsset);
			}
		}

		return nullptr;
	}
};
