// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDBaseStaminaComponent.h"

UVDBaseStaminaComponent::UVDBaseStaminaComponent()
{
	bIsStaminaRecovery = true; // DESC :: 스태미나 회복가능 기본값 
	PrimaryComponentTick.bCanEverTick = true;
}

void UVDBaseStaminaComponent::ConsumeStamina(float StaminaCost)
{
	if (CurrentStamina - StaminaCost >= 0.0f)
	{
		CurrentStamina -= StaminaCost;

		OnChangedStamina.Broadcast(CurrentStamina / MaxStamina);
	}
}

bool UVDBaseStaminaComponent::CanConsumeStamina(float StaminaCost) const
{
	return (CurrentStamina - StaminaCost) >= 0.0f;
}

void UVDBaseStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVDBaseStaminaComponent::SetStaminaRecovery(bool bCanRecover)
{
	bIsStaminaRecovery = bCanRecover;
}

void UVDBaseStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsStaminaRecovery)
	{
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina += DeltaTime;

			OnChangedStamina.Broadcast(CurrentStamina/MaxStamina);
		}
	}
}

