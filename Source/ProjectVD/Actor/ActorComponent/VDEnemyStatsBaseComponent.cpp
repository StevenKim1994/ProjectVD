// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"

void UVDEnemyStatsBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVDEnemyStatsBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetHealth(float InHealth)
{
	Super::SetHealth(InHealth);

	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetMaxHealth(float InMaxHealth, bool bClampCurrent)
{
	Super::SetMaxHealth(InMaxHealth, bClampCurrent);

	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetAttackPower(float InAttackPower)
{
	AttackPower = InAttackPower;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetAttackRange(float InAttackRange)
{
	AttackRange = InAttackRange;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetAttackSpeed(float InAttackSpeed)
{
	AttackSpeed = InAttackSpeed;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetFindPlayerRange(float InFindPlayerRange)
{
	FindPlayerRange = InFindPlayerRange;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetPatrolRange(float InPatrolRange)
{
	PatrolRange = InPatrolRange;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetPatrolWaitTime(float InPatrolWaitTime)
{
	PatrolWaitTime = InPatrolWaitTime;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetTurnSpeed(float InTurnSpeed)
{
	TurnSpeed = InTurnSpeed;
	return this;
}

UVDEnemyStatsBaseComponent* UVDEnemyStatsBaseComponent::SetMaxMovementSpeed(float InMaxMovementSpeed)
{
	MaxMovementSpeed = InMaxMovementSpeed;
	return this;
}

void UVDEnemyStatsBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
