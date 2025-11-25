// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleController.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Title/VDTitlePanelUserWidget.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"
#include "MediaSoundComponent.h"
#include "MediaPlayer.h"


AVDTitleController::AVDTitleController()
{
	TitleBackgroundMediaSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("TitleBackgroundMediaSoundComponent"));
	bShowMouseCursor = true;
}

void AVDTitleController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());

	if (TitleBackgroundMediaPlayer)
	{
		//TitleBackgroundMediaPlayer->OpenSource(TitleBackgroundMediaSource);

		UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();

		if (GI)
		{
			UVDUISubsystem* UISubsystem = GI->GetSubsystem<UVDUISubsystem>();
			if (UISubsystem)
			{
				UISubsystem->AllModalUIWidgetClear();
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
								//TitlePanelUserWidget->SetBackgroundMediaTexture(TitleBackgroundMediaTexture);
								//TitleBackgroundMediaSoundComponent->SetMediaPlayer(TitleBackgroundMediaPlayer);
								TitlePanelUserWidget->OnToggleTitleMovieMuteEvent.BindUObject(this, &AVDTitleController::SetTitleMovieSoundMute);
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
	if (TitleBackgroundMediaSoundComponent)
	{
		TitleBackgroundMediaSoundComponent->SetVolumeMultiplier(bMute ? 0.0f : 1.0f);
	}
}


