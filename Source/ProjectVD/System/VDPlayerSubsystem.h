// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "VDPlayerSubsystem.generated.h"

class AVDCharacterBase;
class UVDInventorySubSystem;
enum class EVDEquipType : uint8;

UCLASS()
class PROJECTVD_API UVDPlayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TWeakObjectPtr<UVDInventorySubSystem> InventorySubsystem;

	UPROPERTY()
	TWeakObjectPtr<AVDCharacterBase> PlayerCharacter;

	UPROPERTY()
	TMap<EVDEquipType, int32> PlayerEquippedMap;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	void SetCurrentCharacter(AVDCharacterBase* InCharacter);
	void SetUseConsumeableItem(int32 ItemID);
	void SetPlayerEquippedItem(EVDEquipType, int32 ItemID);
	FORCEINLINE int32 GetPlayerEquippedItem(EVDEquipType EquipType) const { return PlayerEquippedMap[EquipType]; } ;

};
