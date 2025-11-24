#pragma once

#include "CoreMinimal.h"
#include "FInventoryItem.generated.h"

USTRUCT()
struct PROJECTVD_API FInventoryItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 ItemID = -1;

	UPROPERTY()
	int32 Quantity = 1;

	UPROPERTY()
	int32 MaxQuantity = 99; // DESC :: 한번에 슬롯에 담을 수 있는 최대 수량
};

