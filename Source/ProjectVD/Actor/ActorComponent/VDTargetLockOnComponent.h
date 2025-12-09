// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDTargetLockOnComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FTargetLockOnDelegate, AActor*, /*TargetActor*/ bool /*bIsLockOn*/); // DESC :: 락온 타겟 상태가 바뀌었을때 매 틱마다 호출
DECLARE_MULTICAST_DELEGATE_TwoParams(FTargetLockOnChangedDelegate, AActor*, /*TargetActor*/ bool /*bIsLockOn*/); // DESC :: 락온 타겟 상태가 바뀌었을때 한번 호출

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVD_API UVDTargetLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVDTargetLockOnComponent();
	void ClearLockedOnTarget();
	void SetLockedOnTarget(AActor* NewTarget);
	void LockOnTarget();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FTargetLockOnDelegate OnTargetLockOnChanged;
	FTargetLockOnChangedDelegate OnTargetLockOnStateChanged;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> LockedOnTarget;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> PotentialTargets;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float LockOnRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float LockOnAngle = 45.0f;
};
