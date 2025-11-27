#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VDConsumeableInfo.generated.h"

USTRUCT(BlueprintType)
struct FVDConsumeableInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumeable Info")
	float HealthRestoreAmount; // DESC :: 회복 아이템이 회복하는 체력량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumeable Info")
	float ManaRestoreAmount; // DESC :: 회복 아이템이 회복하는 마나량
};

