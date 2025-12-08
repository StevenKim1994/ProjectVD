// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDHitStopComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVD_API UVDHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVDHitStopComponent();

	void SetHitStop(float InHitStopTime, float TimeDilation);
	void StopHitStop();
protected:
	
private:
	FTimerHandle HitStopTimerHandle;
	void EndHitStop();
};
