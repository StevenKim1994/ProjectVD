// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "VDEnemyStatsBaseComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDEnemyStatsBaseComponent : public UVDBaseStatsComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	float AttackPower;

	UPROPERTY()
	float AttackRange;

	UPROPERTY()
	float AttackSpeed;

	UPROPERTY()
	float FindPlayerRange;

	UPROPERTY()
	float PatrolRange;

	UPROPERTY()
	float PatrolWaitTime;

	UPROPERTY()
	float TurnSpeed;

	UPROPERTY()
	float MaxMovementSpeed;

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	virtual UVDEnemyStatsBaseComponent* SetHealth(float InHealth) override;
	virtual UVDEnemyStatsBaseComponent* SetMaxHealth(float InMaxHealth, bool bClampCurrent = true) override;
	virtual UVDEnemyStatsBaseComponent* SetAttackPower(float InAttackPower);
	virtual UVDEnemyStatsBaseComponent* SetAttackRange(float InAttackRange);
	virtual UVDEnemyStatsBaseComponent* SetAttackSpeed(float InAttackSpeed);
	virtual UVDEnemyStatsBaseComponent* SetFindPlayerRange(float InFindPlayerRange);
	virtual UVDEnemyStatsBaseComponent* SetPatrolRange(float InPatrolRange);
	virtual UVDEnemyStatsBaseComponent* SetPatrolWaitTime(float InPatrolWaitTime);
	virtual UVDEnemyStatsBaseComponent* SetTurnSpeed(float InTurnSpeed);
	virtual UVDEnemyStatsBaseComponent* SetMaxMovementSpeed(float InMaxMovementSpeed);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float GetAttackPower() const { return AttackPower; }
	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }
	FORCEINLINE float GetFindPlayerRange() const { return FindPlayerRange; }
	FORCEINLINE float GetPatrolRange() const { return PatrolRange; }
	FORCEINLINE float GetPatrolWaitTime() const { return PatrolWaitTime; }
	FORCEINLINE float GetTurnSpeed() const { return TurnSpeed; }
	FORCEINLINE float GetMaxMovementSpeed() const { return MaxMovementSpeed; }
};
