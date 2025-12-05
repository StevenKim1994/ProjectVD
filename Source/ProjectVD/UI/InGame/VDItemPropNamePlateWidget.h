// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDItemPropNamePlateWidget.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDItemPropNamePlateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowNamePlate(bool bIsShow);
	void SetItemNameText(const FText& InText);

protected:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void NativeDestruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> RootBorder;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowNamePlateAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideNamePlateAnim;
};
