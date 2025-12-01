// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VDSpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Game/VDGameInstance.h"
#include "System/VDStageObjectSubsystem.h"

AVDSpawnPoint::AVDSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AVDSpawnPoint::SpawnEnemy()
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

		AVDEnemyCharacterBase* SpawnedEnemy = Cast<AVDEnemyCharacterBase>(SpawnedActor);
		if (SpawnedEnemy)
        {
            // TODO :: SubSystem에 등록하기
        }
	}
}

void AVDSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
    FTimerManager& TimerManager = GetWorldTimerManager();
    TimerManager.SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
        {
            this->SpawnEnemy();
        }));
}

void AVDSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

