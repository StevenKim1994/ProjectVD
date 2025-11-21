// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDHUDStaminaWidget.generated.h"

class UProgressBar;
class UVDBaseStaminaComponent;
UCLASS()
class PROJECTVD_API UVDHUDStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY()
	TWeakObjectPtr<UVDBaseStaminaComponent> StaminaComp;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void SetStaminaComp(UVDBaseStaminaComponent* StaminaComp);
	void UpdateStamina(float CurrentStamina);

};
