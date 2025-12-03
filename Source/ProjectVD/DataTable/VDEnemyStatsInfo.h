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
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float FindPlayerRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float PatrolRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float PatrolWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float TurnSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "EnemyStats")
	float MaxMovementSpeed;
};