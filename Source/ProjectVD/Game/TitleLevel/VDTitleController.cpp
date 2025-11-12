// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleController.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Title/VDTitlePanelUserWidget.h"
#include "ETC/VDTitleMovieActor.h"
#include "Game/VDGameInstance.h"
#include "System/VDResourceSystem.h"
AVDTitleController::AVDTitleController()
{
	TitlePanelUserWidgetClass = FSoftClassPath(TEXT("/Game/ProjectVD/UI/TitleUI/TitleUIPanel.TitleUIPanel_C"));
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

	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();

	if (GI)
	{
		UVDResourceSystem* RS = GI->GetSubsystem<UVDResourceSystem>();
		if (RS)
		{
			RS->LoadResourceAsync(TitlePanelUserWidgetClass, [this](UClass* LoadedClass)
				{
					if (LoadedClass)
					{
						TitlePanelUserWidget = CreateWidget<UVDTitlePanelUserWidget>(this, LoadedClass);
						if (TitlePanelUserWidget.IsValid())
						{
							TitlePanelUserWidget->AddToViewport();
							UE_LOG(LogTemp, Log, TEXT("TitlePanelUserWidget Created"));
						}
					}
				});
		}
	}

}

void AVDTitleController::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName)
{
	FSoftClassPath LoadingScreenWidgetRef(TEXT("/Game/ProjectVD/UI/Global/LoadingScreen.LoadingScreen_C"));
	UClass* LoadingScreenWidgetClass = LoadingScreenWidgetRef.TryLoadClass<UUserWidget>();

	if (LoadingScreenWidgetClass)
	{
		UUserWidget* LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
		LoadingScreenWidget->AddToViewport();
		LoadPackageAsync(LevelDir + LevelName,
			FLoadPackageAsyncDelegate::CreateLambda([=, this](const FName& PackageName, UPackage* LoadPackage, EAsyncLoadingResult::Type Result)
				{
					if (Result == EAsyncLoadingResult::Succeeded)
					{
						LoadingScreenWidget->RemoveFromParent();
						AsyncLevelLoadFinished(LevelName);
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("LevelLoad Error"));
					}
				}
			),
			0,
			PKG_ContainsMap);
	}
	else
	{
		UE_LOG(LogTemp, Log,TEXT("LoadingScreen Error"));
	}
}

void AVDTitleController::AsyncLevelLoadFinished(const FString LevelName)
{
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}


