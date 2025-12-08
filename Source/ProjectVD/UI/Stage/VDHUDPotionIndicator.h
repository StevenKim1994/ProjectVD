// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDHUDPotionIndicator.generated.h"

class UTextBlock;
class UImage;
class UOverlay;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDHUDPotionIndicator : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetCoolTimeText(const FText& InText);
	void SetCoolTimeVisibility(bool bIsVisible);
	void SetPotionCount(int32 InCount);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PotionCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PotionIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> CoolTimeOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoolTimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HotkeyText;
};

