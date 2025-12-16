// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDTargetLockOnComponent.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
// Sets default values for this component's properties
UVDTargetLockOnComponent::UVDTargetLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UVDTargetLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UVDTargetLockOnComponent::ClearLockedOnTarget()
{
	SetComponentTickEnabled(false);
	PotentialTargets.Empty();
	LockedOnTarget = nullptr;

	OnTargetLockOnChanged.Broadcast(nullptr, false);
	OnTargetLockOnStateChanged.Broadcast(nullptr, false);
}

void UVDTargetLockOnComponent::SetLockedOnTarget(AVDEnemyCharacterBase* NewTarget)
{
	if (NewTarget == nullptr)
	{
		return;
	}

	LockedOnTarget = NewTarget;
	SetComponentTickEnabled(true);

	OnTargetLockOnStateChanged.Broadcast(LockedOnTarget.Get(), true);
}

void UVDTargetLockOnComponent::LockOnTarget()
{
	if (LockedOnTarget.IsValid())
	{
		ClearLockedOnTarget();
		return;
	}

	PotentialTargets.Empty();
	const float MaxLockOnDistance = 1000.0f; // DESC :: 최대 락온 거리
	const FVector Origin = GetOwner()->GetActorLocation();
	UWorld* World = GetWorld();
	if (World)
	{
		TArray<FOverlapResult> Overlaps;
		FCollisionObjectQueryParams ObjQueryParams;
		ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LockOnOverlap), false, GetOwner());

		const FCollisionShape SphereShape = FCollisionShape::MakeSphere(MaxLockOnDistance);
		const bool bHasOverlaps = World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, ObjQueryParams, SphereShape, QueryParams);

		if (bHasOverlaps)
		{
			float ClosestDistSqr = TNumericLimits<float>::Max();
			for (const FOverlapResult& Result : Overlaps)
			{
				AActor* OverlappedActor = Result.GetActor();
				if (!OverlappedActor || OverlappedActor == GetOwner())
				{
					continue;
				}

				AVDEnemyCharacterBase* EnemyCharacter = Cast<AVDEnemyCharacterBase>(OverlappedActor);
				if (!EnemyCharacter)
				{
					continue;
				}

				const float DistSqr = FVector::DistSquared(Origin, EnemyCharacter->GetActorLocation());
				if (PotentialTargets.Contains(EnemyCharacter) == false)
				{
					PotentialTargets.Add(EnemyCharacter);
					// DESC :: 가장 가까운 Enemy를 현재 잠금 대상에 설정
					if (DistSqr < ClosestDistSqr)
					{
						ClosestDistSqr = DistSqr;
						LockedOnTarget = EnemyCharacter;
						SetLockedOnTarget(LockedOnTarget.Get());
					}
				}
			}
		}
	}
}

void UVDTargetLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LockedOnTarget.IsValid() && !LockedOnTarget.Get()->IsDead())
	{
		OnTargetLockOnChanged.Broadcast(LockedOnTarget.Get(), true);
	}
	else
	{
		ClearLockedOnTarget();
	}
}

