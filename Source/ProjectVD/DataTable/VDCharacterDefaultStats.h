#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"   
#include "VDCharacterDefaultStats.generated.h"

USTRUCT(BlueprintType)
struct FVDCharacterDefaultStats : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackPower;

};