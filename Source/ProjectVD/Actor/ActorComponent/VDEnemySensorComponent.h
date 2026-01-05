// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/VDEnemyInterface.h"
#include "VDEnemySensorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVD_API UVDEnemySensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVDEnemySensorComponent();

private:

	UPROPERTY(VisibleAnywhere, Category = "Enemy Sensor", meta = (AllowPrivateAccess = true))
	TArray<TScriptInterface<IVDEnemyInterface>> DetectedEnemies;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Sensor", meta = (AllowPrivateAccess = true))
	float DetectionRadius = 1000.0f;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE const TArray<TScriptInterface<IVDEnemyInterface>>& GetDetectedEnemies() const { return DetectedEnemies; }
	FORCEINLINE void SetDetectionRadius(float NewRadius) { DetectionRadius = NewRadius; }
		
};
