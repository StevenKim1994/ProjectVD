// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDItemSpawnPoint.generated.h"

class AVDItemPropActorBase;

UCLASS()
class PROJECTVD_API AVDItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AVDItemSpawnPoint();

	FTransform GetSpawnTransform() const { return GetActorTransform(); }
	void SpawnItemProp();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawnPoint", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AVDItemPropActorBase> AllowedClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawnPoint", meta = (AllowPrivateAccess = "true"))
	FTransform SpawnInstanceTransform; // DESC :: 스폰된 액터의 회전 크기, 위치 오프셋용
};
