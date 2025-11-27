#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VDWeaponInfoTable.generated.h"

USTRUCT(BlueprintType)

struct FVDEquipInfoTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Info")
	float Defense; // DESC :: 방어력
};