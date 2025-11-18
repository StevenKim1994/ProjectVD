// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VDSpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Game/VDGameInstance.h"
#include "System/VDStageObjectSubsystem.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#endif
AVDSpawnPoint::AVDSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
    Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
    RootComponent = Billboard;
    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    Arrow->SetupAttachment(RootComponent);
#else
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
#endif
}

void AVDSpawnPoint::SpawnEnemy()
{
    if (UWorld* World = GetWorld())
    {
        if (UVDStageObjectSubsystem* StageObjectSubSystem = World->GetSubsystem<UVDStageObjectSubsystem>())
        {
            if (AllowedClass)
            {
               SpawnedEnemy = Cast<AVDEnemyCharacterBase>(StageObjectSubSystem->Acquire(AllowedClass, GetActorTransform()));

			   if (SpawnedEnemy.IsValid())
               {
				   AVDEnemyCharacterBase* Enemy = SpawnedEnemy.Get();
                   Enemy->SetActorHiddenInGame(true);
                   Enemy->SetActorEnableCollision(false);
                   Enemy->SetActorTickEnabled(false);
               }
            }
        }
    }
}

void AVDSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
    SpawnEnemy();
}

void AVDSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

