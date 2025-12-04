#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EVDLockOnStateType : uint8
{
	LockOff     UMETA(DisplayName = "LockOff"),
	LockOn      UMETA(DisplayName = "LockOn"),
};