// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDHUDStaminaWidget.h"
#include "Components/ProgressBar.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"

void UVDHUDStaminaWidget::SetStaminaComp(UVDBaseStaminaComponent* InStaminaComp)
{
	if (InStaminaComp)
	{
		StaminaComp = InStaminaComp;
		StaminaComp->GetOnChangedStamina().AddUObject(this, &UVDHUDStaminaWidget::UpdateStamina);
	}
}

void UVDHUDStaminaWidget::UpdateStamina(float CurrentStamina)
{
	if(StaminaProgressBar && StaminaComp.Get())
	{
		StaminaProgressBar->SetPercent(CurrentStamina / StaminaComp->GetMaxStamina());
	}

}
