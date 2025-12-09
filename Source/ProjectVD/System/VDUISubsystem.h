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
class UVDToastContainer;
class UVDCutSceneNamePlateWidget;

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
	TObjectPtr<UVDToastContainer> ToastContainerWidget;

	UPROPERTY()
	TObjectPtr<UVDCutSceneNamePlateWidget> CutSceneNamePlateWidget;

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> ModalUIWidgetStack;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CachedPlayerController;



public:
	UVDUISubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UUserWidget* GetUIWidget(const FName& WidgetName,bool isCreate = false);
	void SetPlayerControllerRootUIWidget(APlayerController* PlayerController);
    void ShowUIWidgetAsync(const FName& WidgetName, FOnUIWidgetLoadedDelegate OnLoadedDelegate = nullptr, bool bAutoHUDOff = true);
	UUserWidget* ShowUIWidget(const FName& WidgetName, bool bAutoHUDOff = true);
	void HideCurrentHUDWidget();
	void ShowCurrentHUDWidget();
	void SetCurrentHUDWidget(const FName& HUDWidgetName);
	void HideUIWidget(const FName& WidgetName, bool bAutoHUDOn = true);
	void HideUIWidget(UUserWidget* Widget, bool bAutoHUDOn = true);

	void PopModalUIWidget();
	void AllModalUIWidgetClear();
	void ShowToastMessage(const FText& Message);

	TSoftClassPtr<UUserWidget> GetUIWidgetClassPathByName(const FName& WidgetName);

	void ShowCutSceneNamePlateWidget(const FText& InText);
	void HideCutSceneNamePlateWidget();

	FORCEINLINE bool IsModalUIWidgetStackEmpty() const { return ModalUIWidgetStack.Num() == 0; }
	FORCEINLINE int GetModalUIWidgetCount() const { return ModalUIWidgetStack.Num(); }
	FORCEINLINE UUserWidget* GetCurrentHUDWidget() const { return CurrentHUDWidget; }
	FORCEINLINE UUserWidget* PeekModalUIWidget() const 
	{ 
		return ModalUIWidgetStack.Num() > 0 ? ModalUIWidgetStack.Top().Get() : nullptr; 
	}

	template<typename T>
	FORCEINLINE T* PeekModalUIWidgetAs() const
	{
		static_assert(TIsDerivedFrom<T, UUserWidget>::Value, "T must derive from UUserWidget");
		return Cast<T>(PeekModalUIWidget());
	}
};
