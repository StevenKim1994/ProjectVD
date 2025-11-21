// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VDCharacterBase.generated.h"

class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;
class UVDCharacterStatsBaseComponent;
class UVDBaseStaminaComponent;

UCLASS(Abstract)
class PROJECTVD_API AVDCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DefaultAttackAM;
	// 플레이어 전용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVDCharacterStatsBaseComponent> BaseStatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVDBaseStaminaComponent> StaminaComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

	UFUNCTION()
	virtual void DefaultAttack(const FInputActionValue& Value);

public:	
	AVDCharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE UVDCharacterStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent; }
};
