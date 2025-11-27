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
class UVDInventoryComponent;
class AVDItemPropActorBase;
class AVDEquipItemVisualActor;
class UVDInventoryInfo;

enum class EVDEquipType: uint8;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVDInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<AVDEquipItemVisualActor> EquippedWeapon;

	UPROPERTY()
	TMap<EVDEquipType, FName> EquippedArmorMap;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Look(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Zoom(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Escape(const FInputActionValue& Value);

	UFUNCTION()
	virtual void DefaultAttack(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Inventory(const FInputActionValue& Value);

	virtual void Tick(float DeltaTime) override;

public:	
	AVDCharacterBase();

	UFUNCTION()
	virtual void SetEquippedWeapon(AVDEquipItemVisualActor* NewWeapon);
	virtual bool PickItem(AVDItemPropActorBase* Item);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool UpdateEquippedItem(EVDEquipType EquipType, FName ItemID);
	virtual bool UseConsumeableItem(UVDInventoryInfo* Item); 
	FORCEINLINE UVDCharacterStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent; }
};
