// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/VDMovieRegistry.h"
#include "VDMovieRegistry.h"

TSoftObjectPtr<UMediaSource> UVDMovieRegistry::GetMovieMediaSourceByName(const FName& MovieName) const
{
	if (const TSoftObjectPtr<UMediaSource>* FoundMediaSource = MovieMediaSources.Find(MovieName))
	{
		return *FoundMediaSource;
	}

	return nullptr;
}
