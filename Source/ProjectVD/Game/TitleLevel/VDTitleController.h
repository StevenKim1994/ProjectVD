// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VDTitleController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDTitleController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TSubclassOf<class UVDTitlePanelUserWidget> TitlePanelUserWidgetClass;

	UPROPERTY()
	TWeakObjectPtr<class UVDTitlePanelUserWidget> TitlePanelUserWidget;

public:
	AVDTitleController();

	virtual void BeginPlay() override;
};