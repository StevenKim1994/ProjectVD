#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "VDLoadingPanelUserWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FLoadingCompleteEvent);
DECLARE_DYNAMIC_DELEGATE_OneParam(FLoadingProgressEvent, float, Percent);

UCLASS()
class PROJECTVD_API UVDLoadingPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingDescText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingPercentText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading", meta = (AllowPrivateAccess = "true"))
	float LoadingDescTextChangeInterval = 0.5f;
	float LoadingDescTextChangeCurrentTime = 0.0f;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateLoadingDescText(float DeltaTime);

public:
	UPROPERTY(BlueprintReadWrite, Category = "LoadingEventCallback")
	FLoadingCompleteEvent LoadingCompleteEvent;

	UPROPERTY(BlueprintReadWrite, Category = "LoadingEventCallback")
	FLoadingProgressEvent LoadingProgressEvent;

	UFUNCTION(BlueprintCallable)
	UVDLoadingPanelUserWidget* SetLoadingText(const FText& InText);

	UFUNCTION(BlueprintCallable)
	UVDLoadingPanelUserWidget* SetLoadingPercent(float InPercent);
};
