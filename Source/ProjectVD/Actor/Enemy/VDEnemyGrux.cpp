// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/VDEnemyGrux.h"
#include "Engine/DamageEvents.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"
#include "Animation/VDEnemyAnimInstance.h"

AVDEnemyGrux::AVDEnemyGrux()
{
	PrimaryActorTick.bCanEverTick = true;
	BaseStatsComponent = CreateDefaultSubobject<UVDEnemyStatsBaseComponent>(TEXT("BaseStatsComponent"));
	BaseStatsComponent->RegisterComponent();

	// TODO :: 테이블 로드 기능 추가시 수정 필요

	BaseStatsComponent
		->SetFindPlayerRange(1250.f)
		->SetPatrolRange(500.f)
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

void AVDEnemyGrux::HitReact(const FVector& HitPos)
{
	//Super::HitReact(HitPos);
	FVector CurrentLocation = GetActorLocation();
	FVector HitDirection = (CurrentLocation - HitPos).GetSafeNormal();

	// 액터의 전방 벡터 가져오기
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	// 내적을 통해 방향 판별
	float ForwardDot = FVector::DotProduct(ForwardVector, HitDirection);
	float RightDot = FVector::DotProduct(RightVector, HitDirection);

	UVDEnemyAnimInstance* AnimInstance = Cast<UVDEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	// 방향 체크 (전방, 후방, 좌측, 우측)
	if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	{
		if (ForwardDot > 0)
		{
			// 전방에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Front"));
			AnimInstance->Montage_Play(HitReactAM[3]);
		}
		else
		{
			// 후방에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Back"));
			AnimInstance->Montage_Play(HitReactAM[2]);
		}
	}
	else
	{
		if (RightDot > 0)
		{
			// 우측에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Right"));
			AnimInstance->Montage_Play(HitReactAM[0]);
		}
		else
		{
			// 좌측에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Left"));
			AnimInstance->Montage_Play(HitReactAM[1]);
		}
	}
}

void AVDEnemyGrux::BeginPlay()
{
	Super::BeginPlay();
}

void AVDEnemyGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDEnemyGrux::SetComboInputOn(bool bIsOn)
{
}

void AVDEnemyGrux::DefaultAttackHit()
{
}

float AVDEnemyGrux::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(bIsDead)
	{
		return Result;
	}

	HitReact(DamageCauser->GetActorLocation());

	return Result;
}
