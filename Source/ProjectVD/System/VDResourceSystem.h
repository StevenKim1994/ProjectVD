// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/DataAsset.h"
#include "VDResourceSystem.generated.h"

/**
 * 
 */
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

	void LoadResourceAsync(const TSoftObjectPtr<UObject>& ResourcePtr, TFunction<void(UObject*)> OnLoadedCallback);

	void LoadResourceAsync(const FSoftClassPath& ResourcePath, TFunction<void(UClass*)> OnLoadedCallback);
	UClass* LoadResource(const FSoftClassPath& ResourcePath);

	void LoadResourceAsync(const FSoftObjectPath& ResourcePath, TFunction<void(UObject*)> OnLoadedCallback);

	void LoadPrimaryAssetAsync(const FPrimaryAssetId& AssetId, TFunction<void(UPrimaryDataAsset*)> OnLoadedCallback);
	void LoadPrimaryAsset(const FPrimaryAssetId& AssetId);


	template<typename T>
	T* GetLoadedPrimaryAsset(const FPrimaryAssetId& AssetId) const
	{
		static_assert(TIsDerivedFrom<T, UPrimaryDataAsset>::IsDerived, "T must derive from UPrimaryDataAsset");

		if (LoadedPrimaryAssets.Contains(AssetId))
		{
			return Cast<T>(LoadedPrimaryAssets[AssetId]);
		}
		return nullptr;
	}
};
