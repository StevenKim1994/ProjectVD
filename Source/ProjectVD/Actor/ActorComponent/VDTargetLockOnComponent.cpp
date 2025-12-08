// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDTargetLockOnComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
// Sets default values for this component's properties
UVDTargetLockOnComponent::UVDTargetLockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


void UVDTargetLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UVDTargetLockOnComponent::ClearLockedOnTarget()
{
	PrimaryComponentTick.bCanEverTick = false;
	PotentialTargets.Empty();
	LockedOnTarget = nullptr;
}

void UVDTargetLockOnComponent::SetLockedOnTarget(AActor* NewTarget)
{
	if (NewTarget == nullptr)
	{
		return;
	}

	LockedOnTarget = NewTarget;
	PrimaryComponentTick.bCanEverTick = true;
}

void UVDTargetLockOnComponent::LockOnTarget()
{
	PotentialTargets.Empty();
	const float MaxLockOnDistance = 1000.0f;
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

				const float DistSqr = FVector::DistSquared(Origin, OverlappedActor->GetActorLocation());
				if (PotentialTargets.Contains(OverlappedActor) == false)
				{
					// Enemy 태그가 있는 대상만 잠금 후보에 추가
					if (OverlappedActor->ActorHasTag(TEXT("Enemy")))
					{
						const FName TargetSocketName(TEXT("LockOn"));
						bool bHasTargetSocket = false;

						if (USkeletalMeshComponent* SkeletalMeshComp = OverlappedActor->FindComponentByClass<USkeletalMeshComponent>())
						{
							bHasTargetSocket = SkeletalMeshComp->DoesSocketExist(TargetSocketName);
						}

						if (bHasTargetSocket)
						{
							PotentialTargets.Add(OverlappedActor);
						}

						// 가장 가까운 Enemy를 현재 잠금 대상에 설정
						if (bHasTargetSocket && DistSqr < ClosestDistSqr)
						{
							ClosestDistSqr = DistSqr;
							LockedOnTarget = OverlappedActor;
							SetLockedOnTarget(LockedOnTarget.Get());
						}
					}
				}
			}
		}
	}
}

void UVDTargetLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LockedOnTarget.IsValid())
	{
		FRotator LookAtRotation = (LockedOnTarget->GetActorLocation() - GetOwner()->GetActorLocation()).Rotation();
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;
		GetOwner()->SetActorRotation(FMath::RInterpTo(GetOwner()->GetActorRotation(), LookAtRotation, DeltaTime, 10.0f));
	}
	else
	{
		ClearLockedOnTarget();
	}
}

