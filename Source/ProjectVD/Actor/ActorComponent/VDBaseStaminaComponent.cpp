// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDBaseStaminaComponent.h"

UVDBaseStaminaComponent::UVDBaseStaminaComponent()
{
	bIsStaminaRecovery = true; // DESC :: 스태미나 회복가능 기본값
	PrimaryComponentTick.bCanEverTick = true;
}

void UVDBaseStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// 초기 스태미나 비율 브로드캐스트 (0~1)
	const float StaminaRatio = FMath::Clamp(MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f, 0.f, 1.f);
	OnChangedStamina.Broadcast(StaminaRatio);
}

bool UVDBaseStaminaComponent::ConsumeStamina(float StaminaCost)
{
	if (CurrentStamina < StaminaCost)
	{
		return false;
	}

	const float NewStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0.f, MaxStamina);
	if (!FMath::IsNearlyEqual(NewStamina, CurrentStamina))
	{
		CurrentStamina = NewStamina;
		const float StaminaRatio = FMath::Clamp(MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f, 0.f, 1.f);
		OnChangedStamina.Broadcast(StaminaRatio);
	}

	if(CurrentStamina < MaxStamina)
	{
		SetStaminaRecovery(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("UVDBaseStaminaComponent::ConsumeStamina Current Stamina : %f"), CurrentStamina);

	return true;
}

void UVDBaseStaminaComponent::SetStaminaRecovery(bool bCanRecover)
{
	bIsStaminaRecovery = bCanRecover;
	PrimaryComponentTick.SetTickFunctionEnable(bIsStaminaRecovery);
}

void UVDBaseStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsStaminaRecovery && MaxStamina > 0.f)
	{
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina += DeltaTime;
			CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
			const float StaminaRatio = FMath::Clamp(CurrentStamina / MaxStamina, 0.f, 1.f);
			OnChangedStamina.Broadcast(StaminaRatio);
		}

		if(CurrentStamina >= MaxStamina)
		{
			SetStaminaRecovery(false);
		}
	}
}

