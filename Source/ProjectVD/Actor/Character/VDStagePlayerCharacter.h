// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/VDCharacterBase.h"
#include "InputActionValue.h"
#include "VDStagePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AVDStagePlayerController;

UENUM(BlueprintType)
enum class ECharacterClassType : uint8
{
	None = 0, // Need Select 
	Knight,
};

UCLASS()
class PROJECTVD_API AVDStagePlayerCharacter : public AVDCharacterBase
{
	GENERATED_BODY()

private:
	ECharacterClassType CharacterClass;

protected:
	// 플레이어 전용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCameraComponent;

	UPROPERTY()
	TObjectPtr<AVDStagePlayerController> CastPlayerController;

	UFUNCTION()
	void Escape(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Zoom(const FInputActionValue& Value);

	UFUNCTION()
	void JumpBegin(const FInputActionValue& Value);

	UFUNCTION()
	void JumpEnd(const FInputActionValue& Value);

	UFUNCTION()
	void DefendHold(const FInputActionValue& Value);

	UFUNCTION()
	void DefendCancel(const FInputActionValue& InputActionValue);

	virtual void Move(const FInputActionValue& Value) override;

	virtual void DefaultAttack(const FInputActionValue& Value) override;

public:
	AVDStagePlayerCharacter();

	FORCEINLINE ECharacterClassType GetClassType() const { return CharacterClass; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EquipWeapon(AVDWeapon* NewWeapon) override;
};
