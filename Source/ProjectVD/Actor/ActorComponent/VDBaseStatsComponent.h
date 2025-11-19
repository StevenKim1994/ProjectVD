// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDBaseStatsComponent.generated.h"


UCLASS(Abstract , ClassGroup=("Stats"))
class PROJECTVD_API UVDBaseStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVDBaseStatsComponent();

protected:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess ="true"))
	float MaxHealth;

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:

	UFUNCTION(BlueprintCallable, Category="Stats")
	virtual UVDBaseStatsComponent* SetHealth(float InHealth);

	UFUNCTION(BlueprintCallable, Category="Stats")
	virtual UVDBaseStatsComponent* SetMaxHealth(float InMaxHealth, bool bClampCurrent = true);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	virtual UVDBaseStatsComponent* AddHealth(float Delta);

	UFUNCTION(BlueprintPure, Category="Stats")
	virtual FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category="Stats")
	virtual FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
