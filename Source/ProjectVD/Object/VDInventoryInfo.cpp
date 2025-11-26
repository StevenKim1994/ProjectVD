// Fill out your copyright notice in the Description page of Project Settings.

#include "VDInventoryInfo.h"

UVDInventoryInfo::UVDInventoryInfo()
{
}

UVDInventoryInfo* UVDInventoryInfo::SetIsEmpty(bool bNewIsEmpty)
{
	IsEmpty = bNewIsEmpty;
	if (bNewIsEmpty) // DESC :: 빈슬롯설정시에는 기본값으로 초기화
	{
		ItemID = -1;
		ItemType = EVDItemType::Consumable;
		Quantity = 0;
	}
	return this;
}

UVDInventoryInfo* UVDInventoryInfo::operator+(const UVDInventoryInfo& Other)
{
	// 같은 아이템일 경우에만 수량 합산 (슬롯은 비교하지 않음, 애초에 합친다는 의미는 슬롯과 관계 없이 아이템만 같으면 된다는 뜻)
	if (ItemType == Other.ItemType && ItemID == Other.ItemID && ItemID != -1)
	{
		Quantity += Other.Quantity;
		IsEmpty = false;
	}
	// 다른 아이템이거나 빈 슬롯일 경우 현재 객체는 변경하지 않음
	
	return this;
}

UVDInventoryInfo* UVDInventoryInfo::operator-(const UVDInventoryInfo& Other)
{
	// 같은 아이템일 경우에만 수량 차감
	if (ItemType == Other.ItemType && ItemID == Other.ItemID && ItemID != -1)
	{
		Quantity = FMath::Max(0, Quantity - Other.Quantity);
		if (Quantity <= 0)
		{
			SetIsEmpty(true);
		}
	}
	// 다른 아이템이거나 빈 슬롯일 경우 현재 객체는 변경하지 않음
	
	return this;
}

UVDInventoryInfo* UVDInventoryInfo::operator=(const UVDInventoryInfo& Other)
{
	if (this != &Other)
	{
		ItemType = Other.ItemType;
		ItemID = Other.ItemID;
		Quantity = Other.Quantity;
		MaxQuantity = Other.MaxQuantity;
		IsEmpty = Other.IsEmpty;
		Slot = Other.Slot;
	}
	return this;
}
