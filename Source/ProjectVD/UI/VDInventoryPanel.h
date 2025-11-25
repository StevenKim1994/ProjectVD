// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDInventoryPanel.generated.h"

class UVDInventorySlotWidget;
class UTileView;
class UTextBlock;
class UButton;

DECLARE_DELEGATE(FOnClickExitButtonEvent);

UCLASS()
class PROJECTVD_API UVDInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UTileView* InventoryTileView;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UFUNCTION()
	void OnClickCloseButton();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
