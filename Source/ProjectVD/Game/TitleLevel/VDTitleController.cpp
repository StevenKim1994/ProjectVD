// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Title/VDTitlePanelUserWidget.h"
#include "ETC/VDTitleMovieActor.h"
AVDTitleController::AVDTitleController()
{
	static ConstructorHelpers::FClassFinder<UVDTitlePanelUserWidget> TitlePanelUserWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectVD/UI/TitleUI/TitleUIPanel.TitleUIPanel_C'"));
	if(TitlePanelUserWidgetRef.Class)
	{
		TitlePanelUserWidgetClass = TitlePanelUserWidgetRef.Class;
	}	
	SetShowMouseCursor(true);
}

void AVDTitleController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	TArray<AActor*> WorldActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVDTitleMovieActor::StaticClass(), WorldActorArray);
	TitleMovieActor = Cast<AVDTitleMovieActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AVDTitleMovieActor::StaticClass()));

	ensure(TitleMovieActor);

	TitlePanelUserWidget = CreateWidget<UVDTitlePanelUserWidget>(this, TitlePanelUserWidgetClass);
	if(TitlePanelUserWidget.IsValid())
	{
		TitlePanelUserWidget->OnClickStartButtonEvent.AddLambda([&] {    UGameplayStatics::OpenLevel(this, TEXT("Stage")); });
		TitlePanelUserWidget->OnClickExitButtonEvent.AddLambda([&]{	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);});
		TitlePanelUserWidget->OnToggleTitleMovieMuteEvent.AddLambda([&](bool ChangeState)
			{
				TitleMovieActor->SetChangeState(ChangeState);
			});
		TitlePanelUserWidget->AddToViewport();
	}
}


