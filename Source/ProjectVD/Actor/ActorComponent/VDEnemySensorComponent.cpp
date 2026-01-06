// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDEnemySensorComponent.h"
#include "Interface/VDEnemyInterface.h"
#include "Engine/OverlapResult.h"

UVDEnemySensorComponent::UVDEnemySensorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DetectedEnemies.Empty();
}


void UVDEnemySensorComponent::BeginPlay()
{
	Super::BeginPlay();

	DetectedEnemies.Empty();
}


void UVDEnemySensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DetectionRadius > 0.0f)
	{
		DetectedEnemies.Empty();
		TArray<FOverlapResult> OverlapResults;
		const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(DetectionRadius);
		GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			GetOwner()->GetActorLocation(),
			FQuat::Identity,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
			CollisionShape
		);
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor();
			if(OverlappedActor == nullptr)
			{
				continue;
			}

			if(IVDEnemyInterface* CastingInterface = Cast<IVDEnemyInterface>(OverlappedActor))
			{
				TScriptInterface<IVDEnemyInterface> EnemyInterface;
				EnemyInterface.SetObject(OverlappedActor);
				EnemyInterface.SetInterface(CastingInterface);
				DetectedEnemies.Add(EnemyInterface);

				if (EnemyInterface->IsPreAttacking())
				{
					UE_LOG(LogTemp, Warning, TEXT("Detected Pre-Attacking Enemy: %s"), *OverlappedActor->GetName()); // TODO :: 센싱범위내 적이 공격자세를 취한다면 반격알림 UMG 출력하기 추가하기
					EnemyInterface->ShowPerfectDodgeNotify();
				}
				else
				{
					EnemyInterface->HidePerfectDodgeNotify();
				}
			}
		}
	}
}





