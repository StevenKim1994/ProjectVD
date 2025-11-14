// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VDLoadingController.generated.h"

class UVDLoadingPanelUserWidget;

UCLASS()
class PROJECTVD_API AVDLoadingController : public APlayerController
{
	GENERATED_BODY()
	
private:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UVDLoadingPanelUserWidget* LoadingPanelWidget;

public:
	AVDLoadingController();

	void ShowLoadingPanel();
	void HideLoadingPanel();
	void UpdateLoadingPercent(float InPercent);
};
