// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDPlayerSubsystem.h"
#include "Public/VDEquipType.h"

void UVDPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetWorld()->GetFirstPlayerController()->GetPawn<AVDCharacterBase>();
	// TODO :: 이거 여기서 하면안될듯 타이틀씬에서 이미 서브시스템은 초기화되니 .. 별도 메서드로 분리해서 레벨바뀌면 수동호출해야할듯
}

void UVDPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UVDPlayerSubsystem::SetUseConsumeableItem(int32 ItemID)
{
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

}
