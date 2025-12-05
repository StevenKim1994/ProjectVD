#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "VDWeaponInfoTable.generated.h"

USTRUCT(BlueprintType)
struct FVDWeaponInfoTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	float AttackPower; // DESC :: 무기 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	float AttackSpeed; // DESC :: 무기 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	float CriticalChance; // DESC :: 치명타 확률

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	TSoftClassPtr<AVDEquipItemVisualActor> WeaponVisualActorClass; // DESC :: 무기 시각적 액터 클래스
};