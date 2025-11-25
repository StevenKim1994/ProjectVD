// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Object/VDInventoryInfo.h"
#include "VDInventorySubSystem.generated.h"

UCLASS()
class PROJECTVD_API UVDInventorySubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<int32, TObjectPtr<UVDInventoryInfo>> InventoryMap; // DESC :: 인벤토리 슬롯 , 아이템정보 구조체 만약 슬롯에 구조체가 없다면 빈슬롯

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UVDInventorySubSystem();

	void SetInventoryItemBySlot(int32 Slot, UVDInventoryInfo* Item);
	void RemoveInventoryItemBySlot(int32 Slot);
	void ClearInventory();
	void AddInventoryItem(const UVDInventoryInfo& Item);
	bool IsInventoryFull() const;
	FORCEINLINE const UVDInventoryInfo* GetInventoryItemBySlot(int32 Slot) const { return InventoryMap.Find(Slot)->Get(); }
	FORCEINLINE const TMap<int32, TObjectPtr<UVDInventoryInfo>>& GetInventoryMap() const { return InventoryMap; };
};
