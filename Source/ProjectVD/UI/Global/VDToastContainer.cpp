// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Global/VDToastContainer.h"
#include "UI/Global/VDToastEntryWidget.h"
#include "Components/VerticalBox.h"
#include "Blueprint/UserWidget.h"

void UVDToastContainer::HandleToastFinished(UVDToastEntryWidget* FinishedToast)
{
	if (!FinishedToast)
	{
		return;
	}

	ActiveToasts.Remove(FinishedToast);

	if (ToastStack)
	{
		ToastStack->RemoveChild(FinishedToast);
	}

	ToastPool.Add(FinishedToast);
	FinishedToast->SetVisibility(ESlateVisibility::Collapsed);

	// 대기 중인 토스트가 있으면 다음 토스트 표시
	if (!PendingQueue.IsEmpty())
	{
		FToastRequest NextRequest;
		if (PendingQueue.Dequeue(NextRequest))
		{
			SpawnToast(NextRequest);
		}
	}
}

void UVDToastContainer::SpawnToast(const FToastRequest& Request)
{
	if (!ToastStack)
	{
		return;
	}

	// 최대 토스트 수 확인
	if (ActiveToasts.Num() >= MaxToasts)
	{
		// 큐에 다시 추가
		PendingQueue.Enqueue(Request);
		return;
	}

	UVDToastEntryWidget* ToastWidget = nullptr;

	// 풀에서 재사용 가능한 위젯 가져오기
	if (ToastPool.Num() > 0)
	{
		ToastWidget = ToastPool.Pop();
	}
	else if (ToastEntryWidgetClass)
	{
		// 새 위젯 생성
		ToastWidget = CreateWidget<UVDToastEntryWidget>(this, ToastEntryWidgetClass);

		if (ToastWidget)
		{
			UVDToastEntryWidget* ToastWidgetPtr = Cast<UVDToastEntryWidget>(ToastWidget);
			if (ToastWidgetPtr)
			{
				ToastWidgetPtr->OnToastAnimationFinished().BindUObject(this, &UVDToastContainer::HandleToastFinished);
			}
		}
	}

	if (!ToastWidget)
	{
		return;
	}

	ToastWidget
		->SetToastMessageText(Request.Message)
		->SetToastDuration(Request.Duration)
		->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	ToastStack->AddChildToVerticalBox(ToastWidget);
	ActiveToasts.Add(ToastWidget);
}

void UVDToastContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ToastPool.Empty();
}

void UVDToastContainer::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDToastContainer::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDToastContainer::ShowToast(const FText& Message)
{
	SpawnToast(FToastRequest{ Message, nullptr, ToastDuration });
}

void UVDToastContainer::ShowToast(const FText& Message, float Duration)
{
	SpawnToast(FToastRequest{ Message, nullptr, Duration });
}
