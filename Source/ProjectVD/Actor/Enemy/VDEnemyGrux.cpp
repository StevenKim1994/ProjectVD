// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/VDEnemyGrux.h"
#include "Engine/DamageEvents.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"

AVDEnemyGrux::AVDEnemyGrux()
{
	PrimaryActorTick.bCanEverTick = true;
	BaseStatsComponent = CreateDefaultSubobject<UVDEnemyStatsBaseComponent>(TEXT("BaseStatsComponent"));
	BaseStatsComponent->RegisterComponent();

	// TODO :: 테이블 로드 기능 추가시 수정 필요

	BaseStatsComponent
		->SetFindPlayerRange(100.f)
		->SetPatrolRange(50.f)
		->SetPatrolWaitTime(2.f)
		->SetTurnSpeed(5.f)
		->SetAttackRange(100.f)
		->SetAttackSpeed(1.0f)
		->SetAttackPower(15.f)
		->SetMaxHealth(110.f)
		->SetHealth(110.f);
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
