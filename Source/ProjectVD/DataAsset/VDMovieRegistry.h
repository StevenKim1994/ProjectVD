// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VDMovieRegistry.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDMovieRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Movie", GetFName());
	}

	UFUNCTION(BlueprintCallable, Category = "Movie")
	TSoftObjectPtr<UMediaSource> GetMovieMediaSourceByName(const FName& MovieName) const;

private:
	UPROPERTY(EditAnywhere, Category = "Movies", meta = (AllowedClasses = "/Script/MediaAssets.MediaSource"))
	TMap<FName, TSoftObjectPtr<UMediaSource>> MovieMediaSources;
};
