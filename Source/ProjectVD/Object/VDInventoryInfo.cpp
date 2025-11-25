// Fill out your copyright notice in the Description page of Project Settings.

#include "VDInventoryInfo.h"

UVDInventoryInfo::UVDInventoryInfo()
{
}

UVDInventoryInfo* UVDInventoryInfo::operator+(const UVDInventoryInfo& Other) const
{
	UVDInventoryInfo* Result = NewObject<UVDInventoryInfo>();
	
	// 같은 아이템일 경우에만 수량 합산
	if (ItemID == Other.ItemID && ItemID != -1)
	{
		Result->SetItemID(ItemID);
		Result->SetQuantity(Quantity + Other.Quantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(false);
	}
	else
	{
		// 다른 아이템이거나 빈 슬롯일 경우 현재 객체 복사
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
	if (ItemID == Other.ItemID && ItemID != -1)
	{
		int32 NewQuantity = FMath::Max(0, Quantity - Other.Quantity);
		Result->SetItemID(ItemID);
		Result->SetQuantity(NewQuantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(NewQuantity <= 0);
	}
	else
	{
		// 다른 아이템이거나 빈 슬롯일 경우 현재 객체 복사
		Result->SetItemID(ItemID);
		Result->SetQuantity(Quantity);
		Result->SetMaxQuantity(MaxQuantity);
		Result->SetIsEmpty(IsEmpty);
	}
	
	return Result;
}
