// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "VDCharacterStatsBaseComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDCharacterStatsBaseComponent : public UVDBaseStatsComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxMana;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	FOnChangeStateStats OnChangeMana;

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	UVDCharacterStatsBaseComponent* SetMana(float InMana);
	UVDCharacterStatsBaseComponent* SetMaxMana(float InMaxMana, bool bClampCurrent = true);
	UVDCharacterStatsBaseComponent* AddMana(float Delta);
	
	virtual UVDCharacterStatsBaseComponent* AddHealth(float Delta) override;
	virtual UVDCharacterStatsBaseComponent* SetHealth(float InHealth) override;
	virtual UVDCharacterStatsBaseComponent* SetMaxHealth(float InMaxHealth, bool bClampCurrent = true) override;
	virtual UVDCharacterStatsBaseComponent* SetAttackPower(float InAttackPower);
	virtual UVDCharacterStatsBaseComponent* SetAttackRange(float InAttackRange);
	virtual UVDCharacterStatsBaseComponent* SetAttackSpeed(float InAttackSpeed);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float GetMana() const { return Mana; }
	FORCEINLINE float GetMaxMana() const { return MaxMana; }
	FORCEINLINE float GetAttackPower() const { return AttackPower; }
	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }
	FORCEINLINE FOnChangeStateStats& GetOnChangeMana() { return OnChangeMana; }
};
