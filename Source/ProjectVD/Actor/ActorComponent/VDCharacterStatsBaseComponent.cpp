// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

void UVDCharacterStatsBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVDCharacterStatsBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Mana = MaxMana;
}

UVDCharacterStatsBaseComponent::UVDCharacterStatsBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetMana(float InMana)
{
	Mana = FMath::Clamp(InMana, 0.f, MaxMana);
	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetMaxMana(float InMaxMana, bool bClampCurrent)
{
	MaxMana = FMath::Max(0.f, InMaxMana);
	if (bClampCurrent)
	{
		Mana = FMath::Clamp(Mana, 0.f, MaxMana);
	}
	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::AddMana(float Delta)
{
	Mana = FMath::Clamp(Mana + Delta, 0.f, MaxMana);
	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::AddHealth(float Delta)
{
	Super::AddHealth(Delta);

	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetHealth(float InHealth)
{
	Super::SetHealth(InHealth);

	OnChangeHealth.Broadcast(Health, MaxHealth);
	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetMaxHealth(float InMaxHealth, bool bClampCurrent)
{
	Super::SetMaxHealth(InMaxHealth, bClampCurrent);

	OnChangeHealth.Broadcast(Health, MaxHealth);
	return this;	
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetAttackPower(float InAttackPower)
{
	AttackPower = FMath::Max(0.f, InAttackPower);

	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetAttackRange(float InAttackRange)
{
	AttackRange = FMath::Max(0.f, InAttackRange);

	return this;
}

UVDCharacterStatsBaseComponent* UVDCharacterStatsBaseComponent::SetAttackSpeed(float InAttackSpeed)
{
	AttackSpeed = FMath::Max(0.f, InAttackSpeed);

	return this;
}

