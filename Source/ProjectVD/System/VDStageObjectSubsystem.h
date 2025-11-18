// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VDStageObjectSubsystem.generated.h"


struct FActorPool;
class AVDWeapon;
class AVDSpawnPoint;

UCLASS()
class PROJECTVD_API UVDStageObjectSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
    UPROPERTY()
	TArray<AVDSpawnPoint*> SpawnPoints;

    UPROPERTY() 
    TMap<TSubclassOf<AActor>, FActorPool> Pools;

    AActor* SpawnNew(TSubclassOf<AActor> ActorClass);
    void  DeactivatePooledActor(AActor* Actor);
    void  ActivatePooledActor(AActor* Actor, const FTransform& TM, AActor* Owner, APawn* Instigator);

	void FindingSpawnPoints();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION()
    void InitPool(TSubclassOf<AActor> ActorClass, int32 PrewarmCount = 0, int32 Capacity = 32, bool bAutoExpand = true);

    UFUNCTION()
    AActor* Acquire(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTM, AActor* Owner = nullptr, APawn* Instigator = nullptr);

    UFUNCTION()
    void Release(AActor* Actor);

    UFUNCTION()
    void ReleaseAll(TSubclassOf<AActor> ActorClass);

    UFUNCTION()
    void Shrink(TSubclassOf<AActor> ActorClass, int32 KeepInactive = 0);
};
