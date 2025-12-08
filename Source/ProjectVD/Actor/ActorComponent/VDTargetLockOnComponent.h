// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDTargetLockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVD_API UVDTargetLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVDTargetLockOnComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void ClearLockedOnTarget();
	void SetLockedOnTarget(AActor* NewTarget);
	void LockOnTarget();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TWeakObjectPtr<AActor> LockedOnTarget;
	TArray<TWeakObjectPtr<AActor>> PotentialTargets;
};
