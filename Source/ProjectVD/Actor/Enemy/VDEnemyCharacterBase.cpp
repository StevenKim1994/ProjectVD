// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "Animation/VDEnemyAnimInstance.h"
#include "Public/VDPhysicInfo.h"

AVDEnemyCharacterBase::AVDEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AVDEnemyAIController::StaticClass();

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->InitCapsuleSize(42.0f, 96.0f);
	Capsule->SetCollisionProfileName(CPROFILE_CHARACTER_CAPSULE);

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->SetCollisionProfileName(CPROFILE_NO_COLLISION);

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f); 
	Movement->MaxWalkSpeed = 300.0f; 
	Movement->MinAnalogWalkSpeed = 20.0f;
	Movement->BrakingDecelerationWalking = 2000.0f;
}

UVDEnemyAnimInstance* AVDEnemyCharacterBase::PrepareAnimMontagePlay()
{
	if (FindPlayerAM)
	{
		GetCharacterMovement()->StopMovementImmediately();
		UVDEnemyAnimInstance* AnimInstance = Cast<UVDEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FindPlayerAM);
			return AnimInstance;
		}
	}

	return nullptr;
}

UAnimMontage* AVDEnemyCharacterBase::GetFindPlayerAnimMontage() const
{
	return FindPlayerAM;
}

float AVDEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(BaseStatsComponent)
	{
		float NewHealth = BaseStatsComponent->GetHealth() - DamageAmount;
		BaseStatsComponent->SetHealth(FMath::Clamp(NewHealth, 0.0f, BaseStatsComponent->GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f / %f"), BaseStatsComponent->GetHealth(), BaseStatsComponent->GetMaxHealth());
		if (BaseStatsComponent->GetHealth() <= 0.0f)
		{
			Die();
		}
	}

	return DamageAmount;
}

float AVDEnemyCharacterBase::GetPatrolRadius() const
{
	return BaseStatsComponent->GetPatrolRange();
}

float AVDEnemyCharacterBase::GetPatrolWaitTime() const
{
	return BaseStatsComponent->GetPatrolWaitTime();
}

float AVDEnemyCharacterBase::GetFindingRange() const
{
	return BaseStatsComponent->GetFindPlayerRange();
}

float AVDEnemyCharacterBase::GetTurnSpeed() const
{
	return BaseStatsComponent->GetTurnSpeed();
}

float AVDEnemyCharacterBase::GetAttackRadius() const
{
	return BaseStatsComponent->GetAttackRange();
}

float AVDEnemyCharacterBase::GetAttackRange() const
{
	return BaseStatsComponent->GetAttackRange();
}

void AVDEnemyCharacterBase::FindPlayer()
{
	if(bIsDead)
	{
		return;
	}

	if (FindPlayerAM)
	{
		GetCharacterMovement()->StopMovementImmediately();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FindPlayerAM);
		}
	}
}

void AVDEnemyCharacterBase::Move(const FVector& Direction, float Value)
{

}

void AVDEnemyCharacterBase::Die()
{
	bIsDead = true;
	AVDEnemyAIController * AICon = Cast<AVDEnemyAIController>(GetController());
	if (AICon)
	{
		AICon->StopAI();
	}
	if (DeathAM)
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->StopMovementImmediately();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AVDEnemyCharacterBase::EndDieAM);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathAM);
			AnimInstance->Montage_Play(DeathAM);
		}
	}

	// TODO :: 죽음처리를 위해 컴포넌트 비활성화 등 추가 작업 필요
}

void AVDEnemyCharacterBase::DefaultAttack()
{
	if (bIsDead)
	{
		return;
	}

	if (DefaultAttackAM)
	{
		GetCharacterMovement()->StopMovementImmediately();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DefaultAttackAM);
		}
	}
}

void AVDEnemyCharacterBase::HitReact(const FVector& HitPos)
{
	if (bIsDead)
	{
		return;
	}

	// TODO :: 히트 리액트 애니메이션은 자식에서 처리함. 공통으로 필요시 ( 방향 애니메이션이 다 존재하지 않으면 ) 여기에 구현 필요
}

void AVDEnemyCharacterBase::EndDieAM(UAnimMontage* AnimMontage, bool bInterept)
{
	// TODO :: 죽음 애니메이션 종료 후 처리 (액터 제거 등)
	Destroy();
}

void AVDEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	bIsDead = false;
}

void AVDEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

