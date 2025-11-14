// Fill out your copyright notice in the Description page of Project Settings.


#include "ETC/VDTitleMovieActor.h"
#include	"MediaSoundComponent.h"

// Sets default values
AVDTitleMovieActor::AVDTitleMovieActor()
{
	TitleMovieSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("TitleMovieSoundComponent"));
	TitleMovieSoundComponent->RegisterComponent();
	TitleMovieSoundComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AVDTitleMovieActor::BeginPlay()
{
	Super::BeginPlay();
}

void AVDTitleMovieActor::SetTitleMovieSoundMute(bool bMute)
{
	if (bMute)
	{
		TitleMovieSoundComponent->SetVolumeMultiplier(0.0f);
	}
	else
	{
		TitleMovieSoundComponent->SetVolumeMultiplier(1.0f);
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

