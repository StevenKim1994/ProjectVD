// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDHUDStaminaWidget.h"
#include "Components/ProgressBar.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"

void UVDHUDStaminaWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVDHUDStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDHUDStaminaWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDHUDStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UVDHUDStaminaWidget::SetStaminaComp(UVDBaseStaminaComponent* InStaminaComp)
{
	if (InStaminaComp)
	{
		StaminaComp = InStaminaComp;
		StaminaComp->GetOnChangedStamina().RemoveAll(this);
		StaminaComp->GetOnChangedStamina().AddUObject(this, &UVDHUDStaminaWidget::UpdateStamina);
	}
}

void UVDHUDStaminaWidget::UpdateStamina(float CurrentStamina)
{
	if(StaminaProgressBar)
	{
		if (StaminaComp.IsValid() == false || StaminaComp.Get() == nullptr)
		{
			return;
		}

		StaminaProgressBar->SetPercent(CurrentStamina);
	}

}
