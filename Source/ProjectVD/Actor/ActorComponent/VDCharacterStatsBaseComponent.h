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

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Stats")
	UVDCharacterStatsBaseComponent* SetMana(float InMana);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	UVDCharacterStatsBaseComponent* SetMaxMana(float InMaxMana, bool bClampCurrent = true);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	UVDCharacterStatsBaseComponent* AddMana(float Delta);
	
	virtual UVDCharacterStatsBaseComponent* AddHealth(float Delta) override;
	virtual UVDCharacterStatsBaseComponent* SetHealth(float InHealth) override;
	virtual UVDCharacterStatsBaseComponent* SetMaxHealth(float InMaxHealth, bool bClampCurrent = true) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetMana() const { return Mana; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetMaxMana() const { return MaxMana; }

};
