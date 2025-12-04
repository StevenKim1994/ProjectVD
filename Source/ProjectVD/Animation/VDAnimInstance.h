// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VDAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class PROJECTVD_API UVDAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UVDAnimInstance();

	void SetIsLockOnTarget(uint8 bIsLockOn);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character");
	float JumpingThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat");
	uint8 bIsLockOnTarget : 1;
};
