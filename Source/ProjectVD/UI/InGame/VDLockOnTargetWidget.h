// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDLockOnTargetWidget.generated.h"


class UImage;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDLockOnTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnTargetLockOnChanged(AActor* TargetActor, bool bIsLocked);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootPanel;

	UPROPERTY(meta = (BindWidget))
	UImage* LockOnTargetImage;

	UPROPERTY()
	TWeakObjectPtr<AActor> LockedOnTargetActor;

};
