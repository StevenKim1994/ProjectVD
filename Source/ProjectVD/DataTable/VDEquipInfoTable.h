#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VDEquipInfoTable.generated.h"

USTRUCT(BlueprintType)

struct FVDEquipInfoTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Info")
	float Defense = 0.f;
};