// Fill out your copyright notice in the Description page of Project Settings.


#include "ETC/VDTitleMovieActor.h"

// Sets default values
AVDTitleMovieActor::AVDTitleMovieActor()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MediaPlayerRef(TEXT("/Script/MediaAssets.MediaPlayer'/Game/Movies/Title_MoviePlayer.Title_MoviePlayer'"));
	if (MediaPlayerRef.Object)
	{
		TitleMoviePlayer = MediaPlayerRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMediaSource> MediaSourceRef(TEXT("/Script/MediaAssets.FileMediaSource'/Game/Movies/Title_Movie.Title_Movie'"));
	if (MediaSourceRef.Object)
	{
		TitleMovieSource = MediaSourceRef.Object;
	}

	TitleMoviePlayer->OpenSource(TitleMovieSource);
	TitleMovieSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("TitleMovieSound"));
}

// Called when the game starts or when spawned
void AVDTitleMovieActor::BeginPlay()
{
	Super::BeginPlay();

	TitleMovieSoundComponent->SetMediaPlayer(TitleMoviePlayer);
}

void AVDTitleMovieActor::SetChangeState(bool ChangeState)
{
	if(ChangeState)
	{
		TitleMovieSoundComponent->SetMediaPlayer(nullptr);
	}
	else
	{
		TitleMovieSoundComponent->SetMediaPlayer(TitleMoviePlayer);
	}
}


