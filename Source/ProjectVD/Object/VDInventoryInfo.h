// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VDInventoryInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDInventoryInfo : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	int32 ItemID = -1;

	UPROPERTY()
	int32 Quantity = 1;

	UPROPERTY()
	int32 MaxQuantity = 99; // DESC :: 한번에 슬롯에 담을 수 있는 최대 수량

	UPROPERTY()
	bool IsEmpty = true;

public:
	UVDInventoryInfo();

	FORCEINLINE UVDInventoryInfo* SetItemID(int32 NewItemID) { ItemID = NewItemID; return this; }
	FORCEINLINE int32 GetItemID() const { return ItemID; }
	
	FORCEINLINE UVDInventoryInfo* SetQuantity(int32 NewQuantity) { Quantity = NewQuantity; return this; }
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	FORCEINLINE UVDInventoryInfo* SetMaxQuantity(int32 NewMaxQuantity) { MaxQuantity = NewMaxQuantity; return this; }
	FORCEINLINE int32 GetMaxQuantity() const { return MaxQuantity; }

	FORCEINLINE UVDInventoryInfo* SetIsEmpty(bool bNewIsEmpty) { IsEmpty = bNewIsEmpty; return this; }
	FORCEINLINE bool GetIsEmpty() const { return IsEmpty; }

	UVDInventoryInfo* operator+(const UVDInventoryInfo& Other) const;
	UVDInventoryInfo* operator-(const UVDInventoryInfo& Other) const;

};
