// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDPlayerSubsystem.h"
#include "System/VDInventorySubSystem.h"
#include "System/VDDataTableSubSystem.h"
#include "Public/VDEquipType.h"
#include "Public/VDItemType.h"
#include "DataTable/VDItemInfoTable.h"
#include "DataTable/VDWeaponInfoTable.h"
#include "Actor/Character/VDCharacterBase.h"

void UVDPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InventorySubsystem = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>(); // DESC :: 약한참조로 인벤토리 서브시스템 캐싱
}

void UVDPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UVDPlayerSubsystem::SetCurrentCharacter(AVDCharacterBase* InCharacter)
{
	if (!InCharacter)
	{
		return;
	}

	PlayerCharacter = InCharacter;
}

void UVDPlayerSubsystem::SetUseConsumeableItem(FName ItemID)
{
	if (!InventorySubsystem.IsValid())
	{
		return;
	}

	if (!PlayerCharacter.IsValid())
	{
		return;
	}

	UVDInventoryInfo* Item = InventorySubsystem->GetItem(EVDItemType::Consumable, ItemID);

	if (!Item)
	{
		return;
	}

	if (PlayerCharacter->UseConsumeableItem(Item)) // DESC :: 소비아이템 사용 성공
	{
		InventorySubsystem->RemoveInventoryItemBySlot(Item->GetSlot());
	}
}

void UVDPlayerSubsystem::SetPlayerEquippedItem(EVDEquipType EquipType, FName ItemID)
{
	if (PlayerEquippedMap.Contains(EquipType))
	{
		PlayerEquippedMap[EquipType] = ItemID;
	}
	else
	{
		PlayerEquippedMap.Add(EquipType, ItemID);
	}

	if (EquipType == EVDEquipType::Weapon)
	{
		UVDDataTableSubSystem* DataTableSubsystem = GetGameInstance()->GetSubsystem<UVDDataTableSubSystem>(); 
		FVDWeaponInfoTable* WeaponTable = DataTableSubsystem->GetDataTableRow<FVDWeaponInfoTable>(FName(TEXT("WeaponInfo")), ItemID);

		if (!WeaponTable)
		{
			return;
		}
		WeaponTable->WeaponVisualActorClass.LoadSynchronous();
		UClass* LoadedClass = WeaponTable->WeaponVisualActorClass.Get();

		if (LoadedClass)
		{
			AVDEquipItemVisualActor* WeaponActor = GetWorld()->SpawnActor<AVDEquipItemVisualActor>(LoadedClass);
			PlayerCharacter->SetEquippedWeapon(WeaponActor);
		}
	}
	else
	{
		PlayerCharacter->UpdateEquippedItem(EquipType, ItemID);
	}
}
