#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VDEnemyStatsInfo.generated.h"

USTRUCT(BlueprintType)
struct FVDEnemyStatsInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float AttackPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float AttackSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float AttackRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float FindPlayerRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float PatrolRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float PatrolWaitTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float TurnSpeed = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "EnemyStats")
	float MaxMovementSpeed = 0.f;
};