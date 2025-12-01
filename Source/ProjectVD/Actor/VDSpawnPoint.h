// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDSpawnPoint.generated.h"

class AVDEnemyCharacterBase;
UCLASS()
class PROJECTVD_API AVDSpawnPoint : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AVDEnemyCharacterBase> AllowedClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint", meta = (AllowPrivateAccess = "true"))
	FTransform SpawnInstanceTransform; // DESC :: 스폰된 액터의 회전 크기, 위치 오프셋용

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:	
	AVDSpawnPoint();

	FTransform GetSpawnTransform() const { return GetActorTransform(); }
	void SpawnEnemy();
};
