// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDUISubsystem.h"
#include "System/VDResourceSystem.h"
#include "Engine/AssetManager.h"
#include "Public/VDConstrants.h"
#include "DataAsset/VDUIRegistry.h"
#include "Game/VDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Global/VDToastContainer.h"
#include "UI/Stage/VDCutSceneNamePlateWidget.h"

UVDUISubsystem::UVDUISubsystem()
{

}

void UVDUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ResourceSystem = Collection.InitializeDependency<UVDResourceSystem>();
	UIRegistry = ResourceSystem->GetLoadedPrimaryAsset<UVDUIRegistry>(FPrimaryAssetId(FName(TEXT("UI")), FName(TEXT("PAL_UIRegistry"))));
	
	ModalUIWidgetStack.Empty();
	ActiveWidgetInstanceMap.Empty();

	RootUIWidget = GetUIWidget(VDConstants::RootUIWidget, true);

	ToastContainerWidget = Cast<UVDToastContainer>(GetUIWidget(VDConstants::ToastContainer, true));
}

void UVDUISubsystem::Deinitialize()
{
	Super::Deinitialize();

	ModalUIWidgetStack.Empty();
	ActiveWidgetInstanceMap.Empty();
	ResourceSystem = nullptr;
}

/// <summary>
/// 현재 켜져있는 위젯을 반환 만약 isCreate가 true이면 없을경우 새로 생성해서 반환
/// </summary>
/// <param name="WidgetName">UMG 위젯 이름</param>
/// <param name="isCreate">true일시 없으면 꺼져있는 상태를 생성해서 반환</param>
/// <returns>WidgetName에 해당하는 UMG 인스턴스</returns>
UUserWidget* UVDUISubsystem::GetUIWidget(const FName& WidgetName, bool isCreate) 
{
	UUserWidget* ReturnWidget = nullptr;
	if (ActiveWidgetInstanceMap.Contains(WidgetName))
	{
		ReturnWidget = ActiveWidgetInstanceMap[WidgetName];
	}
	else if(isCreate)
	{
		// DESC :: 만약에 위젯이 없으면 새로 생성해서 반환 PlayerController가 없으므로 생성만함.

		TSoftClassPtr<UUserWidget> WidgetClassPtr = GetUIWidgetClassPathByName(WidgetName);
		UClass* WidgetClass = ResourceSystem->LoadResource(WidgetClassPtr.ToString());
		ReturnWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (ReturnWidget)
		{
			ActiveWidgetInstanceMap.Add(WidgetName, ReturnWidget);
		}
	}

	return ReturnWidget;
}

void UVDUISubsystem::SetPlayerControllerRootUIWidget(APlayerController* PlayerController)
{
	CachedPlayerController = PlayerController;

	if (RootUIWidget)
	{
		RootUIWidget->SetOwningPlayer(CachedPlayerController.Get());
		RootUIWidget->AddToViewport();
	}

	if(ToastContainerWidget)
	{
		ToastContainerWidget->SetOwningPlayer(CachedPlayerController.Get());
		ToastContainerWidget->AddToViewport(99);
	}
}

void UVDUISubsystem::ShowUIWidgetAsync(const FName& WidgetName, FOnUIWidgetLoadedDelegate OnLoadedDelegate, bool bAutoHUDOff)
{
	if (!ResourceSystem)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			ResourceSystem = GI->GetSubsystem<UVDResourceSystem>();
		}
	}
	if (!ensure(ResourceSystem)) return;

	TSoftClassPtr<UUserWidget> WidgetClassPtr = GetUIWidgetClassPathByName(WidgetName);
	if (WidgetClassPtr.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget not found in UIRegistry: %s"), *WidgetName.ToString());
		return;
	}

	if (ActiveWidgetInstanceMap.Contains(WidgetName))
	{
		UUserWidget* ExistingWidget = ActiveWidgetInstanceMap[WidgetName];
		if (ModalUIWidgetStack.Contains(ExistingWidget) == false)
		{
			UCanvasPanel* RootContent = Cast<UCanvasPanel>(RootUIWidget->GetRootWidget());
			UCanvasPanelSlot* ExistingSlot = Cast<UCanvasPanelSlot>(RootContent->AddChild(ExistingWidget));

			ExistingSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			ExistingSlot->SetOffsets(FMargin(0.f));
			ModalUIWidgetStack.Push(ExistingWidget);

			if (bAutoHUDOff)
			{
				HideCurrentHUDWidget();
			}

			if (OnLoadedDelegate.IsBound())
			{
				OnLoadedDelegate.Execute(ExistingWidget);
			}
		}
		return;
	}
	else
	{
		FOnClassLoaded OnClassLoadedDelegate;
		OnClassLoadedDelegate.BindLambda(
			[this, WidgetName, OnLoadedDelegate, bAutoHUDOff](UClass* LoadedClass)
			{
				if (!LoadedClass)
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to load widget class asynchronously: %s"), *WidgetName.ToString());
					return;
				}
				if (!this->CachedPlayerController.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("CachedPlayerController is invalid when creating widget: %s"), *WidgetName.ToString());
					return;
				}
				if (UUserWidget* Widget = CreateWidget<UUserWidget>(this->CachedPlayerController.Get(), LoadedClass))
				{
					ActiveWidgetInstanceMap.Add(WidgetName, Widget);
					if (RootUIWidget)
					{
						UCanvasPanel* RootContent = Cast<UCanvasPanel>(RootUIWidget->GetRootWidget());
						UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RootContent->AddChild(Widget));
						Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
						Slot->SetOffsets(FMargin(0.f));
						ModalUIWidgetStack.Push(Widget);
					}

					if (bAutoHUDOff)
					{
						HideCurrentHUDWidget();
					}

					if (OnLoadedDelegate.IsBound())
					{
						OnLoadedDelegate.Execute(Widget);
					}
				}
			});

		const FSoftClassPath ClassPath(WidgetClassPtr.ToString());
		ResourceSystem->LoadResourceAsync(ClassPath, OnClassLoadedDelegate);
	}
}

UUserWidget* UVDUISubsystem::ShowUIWidget(const FName& WidgetName, bool bAutoHUDOff)
{
	UUserWidget* Widget = nullptr;

	if (!ResourceSystem)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			ResourceSystem = GI->GetSubsystem<UVDResourceSystem>();
		}
	}

	ensure(ResourceSystem);

	TSoftClassPtr<UUserWidget> WidgetClassPtr = GetUIWidgetClassPathByName(WidgetName);
	ensure(!WidgetClassPtr.IsValid() || !WidgetClassPtr.IsNull());

	if (ActiveWidgetInstanceMap.Contains(WidgetName))
	{
		Widget = ActiveWidgetInstanceMap[WidgetName];
		if (ModalUIWidgetStack.Contains(Widget) == false)
		{
			UCanvasPanel* RootContent = Cast<UCanvasPanel>(RootUIWidget->GetRootWidget());
			UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RootContent->AddChild(Widget));
			Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			Slot->SetOffsets(FMargin(0.f));

			ModalUIWidgetStack.Push(Widget);

			if (bAutoHUDOff)
			{
				HideCurrentHUDWidget();
			}
		}
	}
	else
	{
		const FSoftClassPath ClassPath(WidgetClassPtr.ToString());
		UClass* WidgetClass = ResourceSystem->LoadResource(ClassPath);

		ensure(WidgetClass);
		ensure(CachedPlayerController.IsValid());
		Widget = CreateWidget<UUserWidget>(CachedPlayerController.Get(), WidgetClass);
		if (Widget)
		{
			ActiveWidgetInstanceMap.Add(WidgetName, Widget);

			if (RootUIWidget)
			{
				UCanvasPanel* RootContent = Cast<UCanvasPanel>(RootUIWidget->GetRootWidget());
				UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RootContent->AddChild(Widget));
				Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
				Slot->SetOffsets(FMargin(0.f));

				ModalUIWidgetStack.Push(Widget);
			}

			if(bAutoHUDOff)
			{
				HideCurrentHUDWidget();
			}
		}
	}

	return Widget;
}

void UVDUISubsystem::HideCurrentHUDWidget()
{
	if (CurrentHUDWidget)
	{
		CurrentHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVDUISubsystem::ShowCurrentHUDWidget()
{
	if (CurrentHUDWidget)
	{
		CurrentHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UVDUISubsystem::SetCurrentHUDWidget(const FName& HUDWidgetName)
{
	if (!ResourceSystem)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			ResourceSystem = GI->GetSubsystem<UVDResourceSystem>();
		}
	}
	TSoftClassPtr<UUserWidget> WidgetClassPtr = GetUIWidgetClassPathByName(HUDWidgetName);
	const FSoftClassPath ClassPath(WidgetClassPtr.ToString());
	UClass* WidgetClass = ResourceSystem->LoadResource(ClassPath);

	ensure(WidgetClass);
	ensure(CachedPlayerController.IsValid());

	if (CurrentHUDWidget)
	{
		CurrentHUDWidget->RemoveFromParent();
		CurrentHUDWidget = nullptr;
	}

	CurrentHUDWidget = CreateWidget<UUserWidget>(CachedPlayerController.Get(), WidgetClass);
	if (CurrentHUDWidget)
	{
		CurrentHUDWidget->AddToViewport();
	}
	
}

void UVDUISubsystem::HideUIWidget(const FName& WidgetName, bool bAutoHUDOn)
{
	if (ActiveWidgetInstanceMap.Contains(WidgetName))
	{
		UUserWidget* Widget = ActiveWidgetInstanceMap[WidgetName];
		Widget->RemoveFromParent();
		if (ModalUIWidgetStack.Contains(Widget))
		{
			ModalUIWidgetStack.Remove(Widget);
		}
	}

	if (bAutoHUDOn)
	{
		if (ModalUIWidgetStack.Num() == 0)
		{
			ShowCurrentHUDWidget();
		}
	}
}

void UVDUISubsystem::HideUIWidget(UUserWidget* Widget, bool bAutoHUDOn)
{
	if (!Widget) return;
	Widget->RemoveFromParent();
	if (ModalUIWidgetStack.Contains(Widget))
	{
		ModalUIWidgetStack.Remove(Widget);
	}

	if (bAutoHUDOn)
	{
		if (ModalUIWidgetStack.Num() == 0)
		{
			ShowCurrentHUDWidget();
		}
	}
}

TSoftClassPtr<UUserWidget> UVDUISubsystem::GetUIWidgetClassPathByName(const FName& WidgetName)
{
	if (UIRegistry.IsValid())
	{
		return UIRegistry->GetWidgetClassByName(WidgetName);
	}
	return nullptr;
}

void UVDUISubsystem::ShowCutSceneNamePlateWidget(const FText& InText)
{
	if (!CutSceneNamePlateWidget)
	{
		TSoftClassPtr<UUserWidget> WidgetClassPtr = GetUIWidgetClassPathByName(VDConstants::CutSceneNamePlateWidget);
		UClass* WidgetClass = ResourceSystem->LoadResource(WidgetClassPtr.ToString());
		CutSceneNamePlateWidget = CreateWidget<UVDCutSceneNamePlateWidget>(GetWorld(), WidgetClass);
		if (CutSceneNamePlateWidget && RootUIWidget)
		{
			UCanvasPanel* RootContent = Cast<UCanvasPanel>(RootUIWidget->GetRootWidget());
			UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RootContent->AddChild(CutSceneNamePlateWidget));
			Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			Slot->SetOffsets(FMargin(0.f));
		}
	}

	if(CutSceneNamePlateWidget)
	{
		CutSceneNamePlateWidget->SetNamePlateText(InText);
		CutSceneNamePlateWidget->SetNamePlateVisibility(true);
	}
}

void UVDUISubsystem::HideCutSceneNamePlateWidget()
{
	if(CutSceneNamePlateWidget)
	{
		CutSceneNamePlateWidget->SetNamePlateVisibility(false);
	}
}

void UVDUISubsystem::PopModalUIWidget()
{
	if (ModalUIWidgetStack.Num() > 0)
	{
		UUserWidget* TopWidget = ModalUIWidgetStack.Pop();
		if (TopWidget)
		{
			TopWidget->RemoveFromParent();
		}
	}
}

void UVDUISubsystem::AllModalUIWidgetClear()
{
	while (ModalUIWidgetStack.Num() > 0)
	{
		UUserWidget* TopWidget = ModalUIWidgetStack.Pop();
		if (TopWidget)
		{
			TopWidget->RemoveFromParent();
		}
	}
}

void UVDUISubsystem::ShowToastMessage(const FText& Message)
{
	if (ToastContainerWidget)
	{
		ToastContainerWidget->ShowToast(Message);
	}
}
