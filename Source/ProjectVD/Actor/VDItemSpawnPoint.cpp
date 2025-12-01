// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VDItemSpawnPoint.h"
#include "Actor/ItemProp/VDItemPropActorBase.h"
// Sets default values
AVDItemSpawnPoint::AVDItemSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	//SpawnInstanceTransform.SetLocation(GetActorLocation());
	//SpawnInstanceTransform.SetRotation(GetActorQuat());
	//SpawnInstanceTransform.SetScale3D(FVector::OneVector);
}

void AVDItemSpawnPoint::SpawnItemProp()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	// TODO :: SubSystem에서 스폰 처리 이전필요 (현재 Actor는 위치만 제공)
	AActor* SpawnedActor = World->SpawnActorDeferred<AActor>(
		AllowedClass,
		SpawnInstanceTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (SpawnedActor)
	{
		SpawnedActor->FinishSpawning(GetSpawnTransform());
		AVDItemPropActorBase* SpawnedItemProp = Cast<AVDItemPropActorBase>(SpawnedActor);
		if (SpawnedItemProp)
		{
			// TODO :: SubSystem에 등록하기
		}
	}
}

void AVDItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
		{
			this->SpawnItemProp();
		}));
}

void AVDItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

