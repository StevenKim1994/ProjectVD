// Fill out your copyright notice in the Description page of Project Settings.

#include "ETC/VDTitleMovieActor.h"
#include "MediaSoundComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AVDTitleMovieActor::AVDTitleMovieActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultRoot;

	TitleMovieSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("TitleMovieSoundComponent"));
	TitleMovieSoundComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVDTitleMovieActor::BeginPlay()
{
	Super::BeginPlay();
}

void AVDTitleMovieActor::SetTitleMovieSoundMute(bool bMute)
{
	if (TitleMovieSoundComponent)
	{
		TitleMovieSoundComponent->SetVolumeMultiplier(bMute ? 0.0f : 1.0f);
	}
}

void AVDTitleMovieActor::SetTitleMovieMediaPlayer(UMediaPlayer* MediaPlayer)
{
	if (TitleMovieSoundComponent)
	{
		TitleMovieSoundComponent->SetMediaPlayer(MediaPlayer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TitleMovieSoundComponent is nullptr"));
	}
}

