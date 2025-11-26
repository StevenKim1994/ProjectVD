// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDToastContainer.generated.h"

class UVerticalBox;
class UVDToastEntryWidget;
class UTexture2D;
USTRUCT()
struct FToastRequest
{
	GENERATED_BODY()

	FText Message;
	TWeakObjectPtr<UTexture2D> Icon;
	float Duration = 2.0f;
};

UCLASS()
class PROJECTVD_API UVDToastContainer : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ToastStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toast", Meta = (AllowPrivateAccess = "true"))
	float ToastDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toast", Meta = (AllowPrivateAccess = "true"))
	int32 MaxToasts = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toast", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ToastEntryWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UVDToastEntryWidget>> ToastPool;

	UPROPERTY()
	TArray<TObjectPtr<UVDToastEntryWidget>> ActiveToasts;

	TQueue<FToastRequest> PendingQueue;

	UFUNCTION()
	void HandleToastFinished(UVDToastEntryWidget* FinishedToast);

	void SpawnToast(const FToastRequest& Request);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void ShowToast(const FText& Message);

};
