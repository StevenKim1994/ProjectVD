// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/VDEnemyGrux.h"
#include "Engine/DamageEvents.h"

AVDEnemyGrux::AVDEnemyGrux()
{
	PrimaryActorTick.bCanEverTick = true;
	BaseStatsComponent = CreateDefaultSubobject<UVDCharacterStatsBaseComponent>(TEXT("BaseStatsComponent"));
	BaseStatsComponent->RegisterComponent();

	BaseStatsComponent
		->SetAttackRange(100.f)
		->SetAttackSpeed(1.0f)
		->SetAttackPower(15.f)
		->SetMaxHealth(110.f)
		->SetMaxMana(50.f)
		->SetHealth(110.f)
		->SetMana(50.f);
}

void AVDEnemyGrux::FindPlayer()
{
	Super::FindPlayer();
}

void AVDEnemyGrux::Move(const FVector& Direction, float Value)
{
	Super::Move(Direction, Value);
}

void AVDEnemyGrux::Die()
{
	Super::Die();
}

void AVDEnemyGrux::DefaultAttack()
{
	Super::DefaultAttack();
}

void AVDEnemyGrux::BeginPlay()
{
	Super::BeginPlay();
}

void AVDEnemyGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDEnemyGrux::SetComboInputOn_Implementation(bool bIsOn)
{
}

void AVDEnemyGrux::DefaultAttackHit_Implementation()
{
}
