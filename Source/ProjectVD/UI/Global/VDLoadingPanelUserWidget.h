#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDLoadingPanelUserWidget.generated.h"

class UTextBlock;
class RichTextBlock;
class UOverlay;

UCLASS()
class PROJECTVD_API UVDLoadingPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UOverlay* LoadingDescOverlay;

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

	void UpdateLoadingDescText(float DeltaTime);

public:
	UVDLoadingPanelUserWidget* SetLoadingText(const FText& InText);
	UVDLoadingPanelUserWidget* SetLoadingPercent(float InPercent);
};
