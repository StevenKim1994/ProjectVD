// Fill out your copyright notice in the Description page of Project Settings.


#include "ETC/VDTitleMovieActor.h"

// Sets default values
AVDTitleMovieActor::AVDTitleMovieActor()
{
	PrimaryActorTick.bCanEverTick = false;

	TitleMoviePlayer = CreateDefaultSubobject<UMediaPlayer>(TEXT("TitleMoviePlayer"));
	TitleMovieSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("TitleMovieSound"));
	TitleMovieSoundComponent->SetupAttachment(RootComponent);
	TitleMovieSoundComponent->SetMediaPlayer(TitleMoviePlayer);
}

// Called when the game starts or when spawned
void AVDTitleMovieActor::BeginPlay()
{
	Super::BeginPlay();
}

void AVDTitleMovieActor::SetChangeState(bool ChangeState)
{
	if(ChangeState)
	{
		TitleMovieSoundComponent->SetVolumeMultiplier(0.0f);
	}
	else
	{
		TitleMovieSoundComponent->SetVolumeMultiplier(1.0f);
	}
}

void AVDTitleMovieActor::SetPlayTitleMovie(UMediaSource* MovieMedia)
{
	if (TitleMoviePlayer && MovieMedia)
	{
		TitleMoviePlayer->OpenSource(MovieMedia);

		TitleMoviePlayer->Play();
	}
}

void AVDTitleMovieActor::SetPauseTitleMovie()
{
	if (TitleMoviePlayer)
	{
		TitleMoviePlayer->Pause();
	}
}


