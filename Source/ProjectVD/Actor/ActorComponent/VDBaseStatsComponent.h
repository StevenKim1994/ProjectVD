// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDBaseStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeStateStats, UVDBaseStatsComponent*, Stats);

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

	// 동적 멀티캐스트는 UPROPERTY로 보관 권장
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnChangeStateStats OnChangeHealth;

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable, Category="Stats")
	virtual UVDBaseStatsComponent* SetHealth(float InHealth);

	UFUNCTION(BlueprintCallable, Category="Stats")
	virtual UVDBaseStatsComponent* SetMaxHealth(float InMaxHealth, bool bClampCurrent = true);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	virtual UVDBaseStatsComponent* AddHealth(float Delta);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category="Stats")
	virtual FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category="Stats")
	virtual FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// 참조 반환은 유지
	virtual FORCEINLINE FOnChangeStateStats& GetOnChangeHealth() { return OnChangeHealth; }
};
