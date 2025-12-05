// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "VDBaseStatsComponent.h"

// Sets default values for this component's properties
UVDBaseStatsComponent::UVDBaseStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UVDBaseStatsComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UVDBaseStatsComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Health = MaxHealth;
}


// Called every frame
void UVDBaseStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UVDBaseStatsComponent* UVDBaseStatsComponent::SetHealth(float InHealth)
{
	Health = FMath::Clamp(InHealth, 0.f, MaxHealth);

	//OnChangeHealth.Broadcast(this);

	return this;
}

UVDBaseStatsComponent* UVDBaseStatsComponent::SetMaxHealth(float InMaxHealth, bool bClampCurrent)
{
	MaxHealth = FMath::Max(0.f, InMaxHealth);
	if (bClampCurrent)
	{
		Health = FMath::Clamp(Health, 0.f, MaxHealth);
	}

	//OnChangeHealth.Broadcast(this);

	return this;
}

UVDBaseStatsComponent* UVDBaseStatsComponent::AddHealth(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);

	//OnChangeHealth.Broadcast(this);

	return this;
}

