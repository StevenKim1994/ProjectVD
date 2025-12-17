// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "VDInventorySlotWidget.generated.h"

class UTextBlock;
class UImage;
class UVDInventoryInfo;

UCLASS()
class PROJECTVD_API UVDInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantityText;

	UPROPERTY()
	TWeakObjectPtr<UVDInventoryInfo> CurrentInventoryInfo;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	void RefreshSlotWidget();
	
};
