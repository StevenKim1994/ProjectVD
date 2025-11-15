// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h" 
#include "DataAsset/VDUIRegistry.h"
#include "VDUISubsystem.generated.h"

class UVDResourceSystem;
class APlayerController; 
class UVDLoadingPanelUserWidget;

DECLARE_DELEGATE_OneParam(FOnUIWidgetLoadedDelegate, UUserWidget* /*LoadedUIWidget*/);

UCLASS()
class PROJECTVD_API UVDUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
    UPROPERTY()
    TSoftObjectPtr<UVDUIRegistry> UIRegistry;

    UPROPERTY()
    TObjectPtr<UVDResourceSystem> ResourceSystem;

	UPROPERTY()
	TMap<FName, TObjectPtr<UUserWidget>> ActiveWidgetInstanceMap;

	UPROPERTY()
	TObjectPtr<UVDLoadingPanelUserWidget> LoadingPanelWidgetInstance;

	UPROPERTY()
	TObjectPtr<UUserWidget> RootUIWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentHUDWidget; // TODO :: 추후 UUserWidget말고 별도의 인터페이스로 변경 고려

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> ModalUIWidgetStack;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CachedPlayerController;

public:
	UVDUISubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UUserWidget* GetUIWidget(const FName& WidgetName);
	void SetPlayerControllerRootUIWidget(APlayerController* PlayerController);
    void ShowUIWidgetAsync(const FName& WidgetName, FOnUIWidgetLoadedDelegate OnLoadedDelegate = nullptr);
	UUserWidget* ShowUIWidget(const FName& WidgetName);
	void HideCurrentHUDWidget();
	void SetCurrentHUDWidget(const FName& HUDWidgetName);

	void HideUIWidget(const FName& WidgetName);

	TSoftClassPtr<UUserWidget> GetUIWidgetClassPathByName(const FName& WidgetName);
};
