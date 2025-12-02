// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDCutSceneNamePlateWidget.generated.h"

class UOverlay;
class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDCutSceneNamePlateWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> NamePlateOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> NamePlateBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NamePlateText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowNamePlateAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideNamePlateAnim;

public:
	UFUNCTION(BlueprintCallable)
	void SetNamePlateText(const FText& InText);

	UFUNCTION(BlueprintCallable)
	void SetNamePlateVisibility(bool bIsVisible);
};
