// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Public/VDItemType.h"
#include "VDInventoryInfo.generated.h"

UCLASS()
class PROJECTVD_API UVDInventoryInfo : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	EVDItemType ItemType = EVDItemType::Consumable;

	UPROPERTY()
	int32 ItemID = -1;

	UPROPERTY()
	int32 Quantity = 1;

	UPROPERTY()
	int32 MaxQuantity = 99; // DESC :: 한번에 슬롯에 담을 수 있는 최대 수량

	UPROPERTY()
	bool IsEmpty = true;

	UPROPERTY()
	int32 Slot = -1; // DESC :: 인벤토리 내 슬롯 번호

public:
	UVDInventoryInfo();

	FORCEINLINE UVDInventoryInfo* SetItemType(EVDItemType NewItemType) { ItemType = NewItemType; return this; }
	FORCEINLINE EVDItemType GetItemType() const { return ItemType; }

	FORCEINLINE UVDInventoryInfo* SetItemID(int32 NewItemID) { ItemID = NewItemID; return this; }
	FORCEINLINE int32 GetItemID() const { return ItemID; }
	
	FORCEINLINE UVDInventoryInfo* SetQuantity(int32 NewQuantity) { Quantity = NewQuantity; return this; }
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	FORCEINLINE UVDInventoryInfo* SetMaxQuantity(int32 NewMaxQuantity) { MaxQuantity = NewMaxQuantity; return this; }
	FORCEINLINE int32 GetMaxQuantity() const { return MaxQuantity; }

	FORCEINLINE UVDInventoryInfo* SetIsEmpty(bool bNewIsEmpty) { IsEmpty = bNewIsEmpty; return this; }
	FORCEINLINE bool GetIsEmpty() const { return IsEmpty; }

	FORCEINLINE UVDInventoryInfo* SetSlot(int32 NewSlot) { Slot = NewSlot; return this; }
	FORCEINLINE int32 GetSlot() const { return Slot; }

	UVDInventoryInfo* operator+(const UVDInventoryInfo& Other) const;
	UVDInventoryInfo* operator-(const UVDInventoryInfo& Other) const;

};
