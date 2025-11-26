// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDPlayerSubsystem.h"
#include "Public/VDEquipType.h"
#include "Actor/Character/VDCharacterBase.h"

void UVDPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UVDPlayerSubsystem::SetCurrentCharacter(AVDCharacterBase* InCharacter)
{
	if(InCharacter)
	{
		PlayerCharacter = InCharacter;
	}
}

void UVDPlayerSubsystem::SetUseConsumeableItem(int32 ItemID)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->UseConsumeableItem(ItemID);
	}
	// TODO :: 소비 아이템 사용 로직 구현 필요
}

void UVDPlayerSubsystem::SetPlayerEquippedItem(EVDEquipType EquipType, int32 ItemID)
{
	if (PlayerEquippedMap.Contains(EquipType))
	{
		// TODO :: 기존 장착아이템 인벤토리 이전 필요
		PlayerEquippedMap[EquipType] = ItemID;
	}
	else
	{
		PlayerEquippedMap.Add(EquipType, ItemID);
	}

	PlayerCharacter->UpdateEquippedItem(EquipType, ItemID);
}
