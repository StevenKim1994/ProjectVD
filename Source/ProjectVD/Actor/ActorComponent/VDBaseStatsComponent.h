// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDBaseStatsComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeStateStats, float, float);

UCLASS(Abstract , ClassGroup=("Stats"))
class PROJECTVD_API UVDBaseStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVDBaseStatsComponent();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess ="true"))
	float MaxHealth;

	FOnChangeStateStats OnChangeHealth;

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	
	virtual UVDBaseStatsComponent* SetHealth(float InHealth);
	virtual UVDBaseStatsComponent* SetMaxHealth(float InMaxHealth, bool bClampCurrent = true);
	virtual UVDBaseStatsComponent* AddHealth(float Delta);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual FORCEINLINE float GetHealth() const { return Health; }
	virtual FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	virtual FORCEINLINE FOnChangeStateStats& GetOnChangeHealth() { return OnChangeHealth; }
};
