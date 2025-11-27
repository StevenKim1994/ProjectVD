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
	TMap<EVDEquipType, FName> PlayerEquippedMap;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	void SetCurrentCharacter(AVDCharacterBase* InCharacter);
	void SetUseConsumeableItem(FName ItemID);
	void SetPlayerEquippedItem(EVDEquipType, FName ItemID);
	FORCEINLINE FName GetPlayerEquippedItem(EVDEquipType EquipType) const { return PlayerEquippedMap[EquipType]; } ;

};
