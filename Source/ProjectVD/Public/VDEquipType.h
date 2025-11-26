#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EVDEquipType : uint8
{
	HeadArmor UMETA(DisplayName = "HeadArmor"),
	BodyArmor UMETA(DisplayName = "BodyArmor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Accessory UMETA(DisplayName = "Accessory"),
};
