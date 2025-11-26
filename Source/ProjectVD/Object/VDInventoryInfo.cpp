// Fill out your copyright notice in the Description page of Project Settings.

#include "VDInventoryInfo.h"

UVDInventoryInfo::UVDInventoryInfo()
{
}

UVDInventoryInfo* UVDInventoryInfo::operator+(const UVDInventoryInfo& Other) const
{
	UVDInventoryInfo* Result = NewObject<UVDInventoryInfo>();
	
	// 같은 아이템일 경우에만 수량 합산 (슬롯은 비교하지 않음, 애초에 합친다는 의미는 슬롯과 관계 없이 아이템만 같으면 된다는 뜻 슬롯은 Result쪽 슬롯으로 합쳐진다.)
	if (ItemType == Other.ItemType && ItemID == Other.ItemID && ItemID != -1)
	{
		Result->SetItemType(ItemType);
		Result->SetItemID(ItemID);
		Result->SetQuantity(Quantity + Other.Quantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(false);
	}
	else
	{
		// 다른 아이템이거나 빈 슬롯일 경우 현재 객체 복사
		Result->SetItemType(ItemType);
		Result->SetItemID(ItemID);
		Result->SetQuantity(Quantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(IsEmpty);
	}
	
	return Result;
}

UVDInventoryInfo* UVDInventoryInfo::operator-(const UVDInventoryInfo& Other) const
{
	UVDInventoryInfo* Result = NewObject<UVDInventoryInfo>();
	
	// 같은 아이템일 경우에만 수량 차감
	if (ItemType == Other.ItemType && ItemID == Other.ItemID && ItemID != -1)
	{
		int32 NewQuantity = FMath::Max(0, Quantity - Other.Quantity);
		Result->SetItemType(ItemType);
		Result->SetItemID(ItemID);
		Result->SetQuantity(NewQuantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(NewQuantity <= 0);
	}
	else
	{
		// 다른 아이템이거나 빈 슬롯일 경우 현재 객체 복사
		Result->SetItemType(ItemType);
		Result->SetItemID(ItemID);
		Result->SetQuantity(Quantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(IsEmpty);
	}
	
	return Result;
}
