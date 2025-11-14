// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleController.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Title/VDTitlePanelUserWidget.h"
#include "ETC/VDTitleMovieActor.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"

AVDTitleController::AVDTitleController()
{
	SetShowMouseCursor(true);
}

void AVDTitleController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());

	if (TitleBackgroundMediaPlayer)
	{
		TitleBackgroundMediaPlayer->OpenSource(TitleBackgroundMediaSource);
		TitleMovieActor = Cast<AVDTitleMovieActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AVDTitleMovieActor::StaticClass()));
		if(TitleMovieActor)
		{
			TitleMovieActor->SetTitleMovieMediaPlayer(TitleBackgroundMediaPlayer);
		}

		UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();

		if (GI)
		{
			UVDUISubsystem* UISubsystem = GI->GetSubsystem<UVDUISubsystem>();
			if (UISubsystem)
			{
				UISubsystem->SetPlayerControllerRootUIWidget(this);
				UISubsystem->ShowUIWidgetAsync(
					VDConstants::TitlePanel,
					FOnUIWidgetLoadedDelegate::CreateWeakLambda(this, [this](UUserWidget* Widget)
					{
						if (Widget)
						{
							TitlePanelUserWidget = Cast<UVDTitlePanelUserWidget>(Widget);
							if (TitlePanelUserWidget.IsValid())
							{
								TitlePanelUserWidget->SetBackgroundMediaTexture(TitleBackgroundMediaTexture);
								TitlePanelUserWidget->OnToggleTitleMovieMuteEvent.AddUObject(this, &AVDTitleController::SetTitleMovieSoundMute);
							}
						}
					})
				);
			}
		}
	}
}

void AVDTitleController::SetTitleBackgroundMovie(bool Pause)
{
	if (TitleBackgroundMediaPlayer)
	{
		if (Pause)
		{
			TitleBackgroundMediaPlayer->Pause();
		}
		else
		{
			TitleBackgroundMediaPlayer->Play();
		}
	}
}

void AVDTitleController::SetTitleMovieSoundMute(bool bMute)
{
	if (TitleMovieActor)
	{
		TitleMovieActor->SetTitleMovieSoundMute(bMute);
	}
}


