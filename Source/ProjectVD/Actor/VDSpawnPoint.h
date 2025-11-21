// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDSpawnPoint.generated.h"

class AVDEnemyCharacterBase;
class UBillboardComponent;
class UArrowComponent;
UCLASS()
class PROJECTVD_API AVDSpawnPoint : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AVDEnemyCharacterBase> AllowedClass;

protected:
#if WITH_EDITORONLY_DATA
	
	UPROPERTY() 
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY() 
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY()
	TWeakObjectPtr<AVDEnemyCharacterBase> SpawnedEnemy;

#endif
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:	
	AVDSpawnPoint();

	FTransform GetSpawnTransform() const { return GetActorTransform(); }
	void SpawnEnemy();
};
