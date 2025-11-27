#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VDItemInfoTable.generated.h"

enum class EVDItemType : uint8;
class UTexture2D;

USTRUCT(BlueprintType)
struct FVDItemInfoTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	int32 MaxQuantity; // DESC :: 한번에 인벤토리 슬롯에 쌓을 수 있는 최대 갯수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FString ItemDescription; // DESC :: 아이템 설명 추후 다국어 지원고려를 위해 Key값으로 변경 고려

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	EVDItemType ItemType; // DESC :: 아이템 타입

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TSoftObjectPtr<UTexture2D> Icon; // DESC :: 아이콘 참조
};
