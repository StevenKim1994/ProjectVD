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
	BaseStatsComponent = CreateDefaultSubobject<UVDEnemyStatsBaseComponent>(TEXT("BaseStatsComponent"));
	HitStopComponent = CreateDefaultSubobject<UVDHitStopComponent>(TEXT("HitStopComponent"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AIControllerClass = AVDEnemyAIController::StaticClass();
	EnemyAIController = Cast<AVDEnemyAIController>(GetController());
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

	Tags.Add(FName("Enemy"));
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

UVDEnemyAnimInstance* AVDEnemyCharacterBase::DefaultAttackMontagePlay(FOnAttackMontageEnded AttackMontageEndedDelegate)
{
	if (DefaultAttackAM)
	{
		GetCharacterMovement()->StopMovementImmediately();
		UVDEnemyAnimInstance* AnimInstance = Cast<UVDEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([AttackMontageEndedDelegate](UAnimMontage* InMontage, bool bInterrupted)
			{
				AttackMontageEndedDelegate.ExecuteIfBound();
			});

			AnimInstance->Montage_Play(DefaultAttackAM);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DefaultAttackAM);

			return AnimInstance;
		}
	}
	return nullptr;
}

UAnimMontage* AVDEnemyCharacterBase::GetFindPlayerAnimMontage() const
{
	return FindPlayerAM;
}

UVDEnemyStatsBaseComponent* AVDEnemyCharacterBase::GetStatsComp() const
{
	return BaseStatsComponent;
}

void AVDEnemyCharacterBase::SetComboInputOn(bool bIsOn)
{

}

void AVDEnemyCharacterBase::DefaultAttackHit()
{

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

		UVDEnemyAnimInstance* AnimInstance = Cast<UVDEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->SetIsDead(1);
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AVDEnemyCharacterBase::EndDieAM);
			AnimInstance->Montage_Play(DeathAM);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathAM);
		}
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
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

}

void AVDEnemyCharacterBase::EndDieAM(UAnimMontage* AnimMontage, bool bInterept)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->StopMovementImmediately();
		Movement->DisableMovement();
		Movement->SetMovementMode(EMovementMode::MOVE_None);
	}

	AVDEnemyAIController* AICon = Cast<AVDEnemyAIController>(GetController());
	if (AICon)
	{
		AICon->StopAI();
		AICon->UnPossess();
	}
	DetachFromControllerPendingDestroy();

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	SetActorTickEnabled(false);
	SetCanBeDamaged(false);
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

