// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/Skeleton/VDEnemySkeleton.h"
#include "VDEnemySkeletonMarauder.generated.h"

class USkeletalMeshComponent;
UCLASS()
class PROJECTVD_API AVDEnemySkeletonMarauder : public AVDEnemySkeleton
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LeftHandWeapon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Shield", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RightHandShield;

protected:

	virtual FName GetEnemyStatsRowKey() const override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void DefaultAttack() override;
	virtual void DefaultAttackHit() override;
public:
	AVDEnemySkeletonMarauder();
};
