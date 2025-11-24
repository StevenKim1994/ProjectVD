// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "VDInventorySlotWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECTVD_API UVDInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIconImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantityText;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
public:
	
};
