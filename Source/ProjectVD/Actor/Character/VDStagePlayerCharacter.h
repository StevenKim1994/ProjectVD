// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/VDCharacterBase.h"
#include "InputActionValue.h"
#include "VDStagePlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EClassType : uint8
{
	None = 0, // Need Select 
	Knight,
};

UCLASS()
class PROJECTVD_API AVDStagePlayerCharacter : public AVDCharacterBase
{
	GENERATED_BODY()

private:
	EClassType CharacterClass;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCameraComponent;

// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RotateLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	void Move(const FInputActionValue& InputValue);
	void RotateLook(const FInputActionValue& InputValue);

public:
	// Sets default values for this character's properties
	AVDStagePlayerCharacter();

public:
	FORCEINLINE EClassType GetClassType() const { return CharacterClass; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
