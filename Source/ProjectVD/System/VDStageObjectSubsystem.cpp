// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDStageObjectSubsystem.h"
#include "Interface/VDPoolableInterface.h"
#include "EngineUtils.h"
#include "Actor/VDSpawnPoint.h"

AActor* UVDStageObjectSubsystem::SpawnNew(TSubclassOf<AActor> ActorClass)
{
	return nullptr;
}

void UVDStageObjectSubsystem::DeactivatePooledActor(AActor* Actor)
{
    if (!IsValid(Actor)) return;

    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);

    // 컴포넌트 정지
    TArray<UActorComponent*> Components;
    Actor->GetComponents(Components);
    for (UActorComponent* C : Components)
    {
        if (!C) continue;
        C->Deactivate();
        C->SetComponentTickEnabled(false);
        if (auto* Primitive = Cast<UPrimitiveComponent>(C))
        {
            Primitive->SetSimulatePhysics(false);
            Primitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }

    if (Actor->Implements<UVDPoolableInterface>())
    {
		// TODO :: 인터페이스 호출 
    }
}

void UVDStageObjectSubsystem::ActivatePooledActor(AActor* Actor, const FTransform& TM, AActor* Owner, APawn* Instigator)
{
}

void UVDStageObjectSubsystem::FindingSpawnPoints()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!IsValid(Actor)) continue;

        bool bIsSpawnPoint = false;

        if (Actor->IsA<AVDSpawnPoint>())
        {
            bIsSpawnPoint = true;
        }

        if (!bIsSpawnPoint) continue;

        // 중복 방지
        if (SpawnPoints.Contains(Actor)) continue;

        SpawnPoints.Add(Cast<AVDSpawnPoint>(Actor));

        UE_LOG(LogTemp, Log, TEXT("UVDStageObjectSubsystem::Initialize - SpawnPoint Registered: %s"), *Actor->GetName());
    }
}

void UVDStageObjectSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    FindingSpawnPoints();
}

void UVDStageObjectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    SpawnPoints.Empty();
}

void UVDStageObjectSubsystem::Deinitialize()
{
	Super::Deinitialize();

	SpawnPoints.Empty();
}

void UVDStageObjectSubsystem::InitPool(TSubclassOf<AActor> ActorClass, int32 PrewarmCount, int32 Capacity, bool bAutoExpand)
{
	if (!*ActorClass)
	{
		return;
	}

	FActorPool& Pool = Pools.FindOrAdd(ActorClass);
	Pool.Capacity = Capacity;
	Pool.DesirePrewarm = PrewarmCount;
	Pool.bAutoExpand = bAutoExpand;

	while (Pool.Inactive.Num() < PrewarmCount)
	{
		if (AActor* NewA = SpawnNew(ActorClass))
		{
			DeactivatePooledActor(NewA);
			Pool.Inactive.Add(NewA);
			Pool.TotalSpawned++;
		}
		else break;
	}
}	

AActor* UVDStageObjectSubsystem::Acquire(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTM, AActor* Owner, APawn* Instigator)
{
    if (!*ActorClass) return nullptr;

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    FActorSpawnParameters Params;
    Params.Owner = Owner;
    Params.Instigator = Instigator;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewActor = World->SpawnActor<AActor>(*ActorClass, SpawnTM, Params);
    return NewActor;
}

void UVDStageObjectSubsystem::Release(AActor* Actor)
{
}

void UVDStageObjectSubsystem::ReleaseAll(TSubclassOf<AActor> ActorClass)
{
}

void UVDStageObjectSubsystem::Shrink(TSubclassOf<AActor> ActorClass, int32 KeepInactive)
{
}
