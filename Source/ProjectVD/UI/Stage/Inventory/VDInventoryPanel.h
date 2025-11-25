// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "VDInventoryPanel.generated.h"

class UVDInventorySlotWidget;
class UTileView;
class UTextBlock;
class UButton;
class UWidgetAnimation;
class UVDInventoryLeftItemDetailSection;

DECLARE_DELEGATE(FOnClickExitButtonEvent);

UCLASS()
class PROJECTVD_API UVDInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTileView> InventoryTileView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVDInventoryLeftItemDetailSection> ItemDetailSection;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RightAnim;

	UPROPERTY(Transient,meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LeftAnim;

	UFUNCTION()
	void OnClickCloseButton();

	UFUNCTION()
	void OnInventoryItemHoveredChanged(UObject* Item, bool bIsHovered);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
