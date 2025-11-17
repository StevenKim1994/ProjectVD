// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDStageObjectSubsystem.h"
#include "Interface/VDPoolableInterface.h"
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

void UVDStageObjectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDStageObjectSubsystem::Deinitialize()
{
	Super::Deinitialize();
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
    FTransform IdentityTM = FTransform::Identity;

    // BeginPlay 전 초기 상태 세팅을 위해 지연 스폰
    AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(*ActorClass, IdentityTM, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (!NewActor) return nullptr;

    // 초기 상태: 숨김/충돌/틱 Off (BeginPlay 전에 보장)
    NewActor->SetActorHiddenInGame(true);
    NewActor->SetActorEnableCollision(false);
    NewActor->SetActorTickEnabled(false);

    // 컴포넌트들도 안전하게 비활성
    TArray<UActorComponent*> Components;
    NewActor->GetComponents(Components);
    for (UActorComponent* C : Components)
    {
        if (C)
        {
            C->Deactivate();
            C->SetComponentTickEnabled(false);
        }
    }

    NewActor->FinishSpawning(IdentityTM);
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
