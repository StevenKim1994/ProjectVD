// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Enemy/Skeleton/VDEnemySkeletonMarauder.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/VDDataTableSubsystem.h"
#include "DataTable/VDEnemyStatsInfo.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h" 
#include "Engine/EngineTypes.h" 
#include "Engine/DamageEvents.h"
#include "Public/VDPhysicInfo.h"

void AVDEnemySkeletonMarauder::BeginPlay()
{
	Super::BeginPlay();
}

void AVDEnemySkeletonMarauder::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	FVDEnemyStatsInfo* DataTableInfo = GameInstance->GetSubsystem<UVDDataTableSubSystem>()->GetDataTableRow<FVDEnemyStatsInfo>(FName(TEXT("EnemyStatsInfo")), FName(TEXT("SkeletonMarauder")));

	if (DataTableInfo)
	{
		BaseStatsComponent
			->SetFindPlayerRange(DataTableInfo->FindPlayerRange)
			->SetPatrolRange(DataTableInfo->PatrolRange)
			->SetPatrolWaitTime(DataTableInfo->PatrolWaitTime)
			->SetTurnSpeed(DataTableInfo->TurnSpeed)
			->SetMaxMovementSpeed(DataTableInfo->MaxMovementSpeed)
			->SetAttackRange(DataTableInfo->AttackRange)
			->SetAttackSpeed(DataTableInfo->AttackSpeed)
			->SetAttackPower(DataTableInfo->AttackPower)
			->SetMaxHealth(DataTableInfo->MaxHealth)
			->SetHealth(DataTableInfo->MaxHealth);
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->MinAnalogWalkSpeed = 50.f;
		Movement->MaxWalkSpeed = BaseStatsComponent->GetMaxMovementSpeed();
	}
}

void AVDEnemySkeletonMarauder::DefaultAttack()
{
	Super::DefaultAttack();
}

void AVDEnemySkeletonMarauder::DefaultAttackHit()
{
	Super::DefaultAttackHit();
	UWorld* World = GetWorld();
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(DefaultAttack), false, this);

	const float AttackRange = BaseStatsComponent->GetAttackRange();
	const float AttackRadius = 50.0f;
	const float AttackDamage = BaseStatsComponent->GetAttackPower();
	const FVector Start = GetActorLocation() + (GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius());
	const FVector End = Start + (GetActorForwardVector() * AttackRange);

	if (World)
	{
		bool bIsHitCheck = World->SweepSingleByChannel(HitResult,
			Start,
			End,
			FQuat::Identity,
			CCHANNEL_PROFILE_CHACRACTER_ACTION,
			FCollisionShape::MakeSphere(50.0f), CollisionParams);

		FVector DebugDrawLocation = Start + (End - Start) * 0.5f;
		float DebugCapsuleHeight = AttackRange * 0.5f;
		FColor DebugColor = bIsHitCheck ? FColor::Red : FColor::Green;

		DrawDebugCapsule(World, DebugDrawLocation, DebugCapsuleHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DebugColor, false, 2.0f);

		if (bIsHitCheck)
		{
			if (HitResult.GetActor())
			{
				if (AVDCharacterBase* HitPlayer = Cast<AVDCharacterBase>(HitResult.GetActor()))
				{
					UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

					FDamageEvent DamageEvent;
					float TakeDamage = BaseStatsComponent->GetAttackPower();
					TakeDamage = HitPlayer->TakeDamage(AttackDamage, DamageEvent, Controller, this);
				}
			}
		}
	}
}

AVDEnemySkeletonMarauder::AVDEnemySkeletonMarauder()
{
	LeftHandWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandWeapon"));
	RightHandShield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandShield"));

	if (LeftHandWeapon)
	{
		LeftHandWeapon->SetupAttachment(GetMesh(),TEXT("hand_lSocket"));
		LeftHandWeapon->SetRelativeLocation(FVector::ZeroVector);
		LeftHandWeapon->SetRelativeRotation(FRotator::ZeroRotator);
		LeftHandWeapon->SetRelativeScale3D(FVector::OneVector);
	}

	if (RightHandShield)
	{
		RightHandShield->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
		RightHandShield->SetRelativeRotation(FRotator::ZeroRotator);
		RightHandShield->SetRelativeLocation(FVector::ZeroVector);
		RightHandShield->SetRelativeScale3D(FVector::OneVector);
	}
}
