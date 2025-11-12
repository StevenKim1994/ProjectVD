// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
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

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void LoadResourceAsync(const TSoftObjectPtr<UObject>& ResourcePtr, TFunction<void(UObject*)> OnLoadedCallback);
	void LoadResourceAsync(const FSoftClassPath& ResourcePath, TFunction<void(UClass*)> OnLoadedCallback);
	void LoadResourceAsync(const FSoftObjectPath& ResourcePath, TFunction<void(UObject*)> OnLoadedCallback);
	
};
