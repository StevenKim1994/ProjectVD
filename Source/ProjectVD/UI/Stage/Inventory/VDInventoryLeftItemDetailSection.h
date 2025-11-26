// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDInventoryLeftItemDetailSection.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UVDInventoryInfo;

UCLASS()
class PROJECTVD_API UVDInventoryLeftItemDetailSection : public UUserWidget
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UVDInventoryInfo> CurrentInventoryInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> RenderAndIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UseButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DeleteButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DeleteButtonText;

	UFUNCTION()
	void OnClickedUseButton();

	UFUNCTION()
	void OnClickedDeleteButton();

	UFUNCTION()
	void OnInventoryChanged(UVDInventoryInfo* ChangedItem);

	void UpdateItemDetailInfo(UVDInventoryInfo* InInventoryInfo);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetItemDetailInfo(UVDInventoryInfo* InInventoryInfo);
	
};
