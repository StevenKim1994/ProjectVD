// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/VDCharacterBase.h"
#include "InputActionValue.h"
#include "VDStagePlayerCharacter.generated.h"

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

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCameraComponent;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void JumpBegin(const FInputActionValue& Value);

	UFUNCTION()
	void JumpEnd(const FInputActionValue& Value);

	UFUNCTION()
	void DefaultAttack(const FInputActionValue& Value);

	UFUNCTION()
	void DefendHold(const FInputActionValue& Value);

	UFUNCTION()
	void DefendCancel(const FInputActionValue& InputActionValue);

public:
	AVDStagePlayerCharacter();

public:
	FORCEINLINE ECharacterClassType GetClassType() const { return CharacterClass; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
