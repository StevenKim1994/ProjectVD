// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDBaseStaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedStamina, float /*StaminaCost*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVD_API UVDBaseStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Stamina")
	bool bIsStaminaRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;

	FOnChangedStamina OnChangedStamina;

public:	
	UVDBaseStaminaComponent();

	void SetMaxStamina(float NewMaxStamina) { MaxStamina = NewMaxStamina; }
	void SetCurrentStamina(float NewCurrentStamina) { CurrentStamina = NewCurrentStamina; }
	void ConsumeStamina(float StaminaCost);
	bool CanConsumeStamina(float StaminaCost) const;
	
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE bool IsStaminaRecovery() const { return bIsStaminaRecovery; }
	FORCEINLINE FOnChangedStamina& GetOnChangedStamina() { return OnChangedStamina; }
	
protected:
	virtual void BeginPlay() override;

public:	

	void SetStaminaRecovery(bool bCanRecover);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
