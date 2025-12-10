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
class UVDTargetLockOnComponent;
class UVDInventoryComponent;
class UVDAnimInstance;
class UVDInventoryInfo;
class AVDItemPropActorBase;
class AVDEquipItemVisualActor;
class AVDStagePlayerController;
enum class EVDEquipType: uint8;

UCLASS(Abstract)
class PROJECTVD_API AVDCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AVDStagePlayerController> CastPlayerController;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DefaultAttackAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RootingAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeathAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DrinkPotionAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DefenceHitAM;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVDTargetLockOnComponent> TargetLockOnComponent;

	UPROPERTY()
	TObjectPtr<AVDEquipItemVisualActor> EquippedWeapon;

	UPROPERTY()
	TObjectPtr<UVDAnimInstance> CastingAnimInstance;

	UPROPERTY()
	TMap<EVDEquipType, FName> EquippedArmorMap;

	UPROPERTY()
	TArray<TWeakObjectPtr<AVDItemPropActorBase>> OverlappingItemList;

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

	UFUNCTION()
	virtual void LockOnTarget(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Rooting(const FInputActionValue& Value);

	UFUNCTION()
	virtual void DrinkPotion(const FInputActionValue& Value);

	UFUNCTION()
	virtual void RollRight(const FInputActionValue& Value);

	UFUNCTION()
	virtual void RollLeft(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Defence(const FInputActionValue& Value);

	UFUNCTION()
	virtual void WeaponColiderHit(AActor* OtherActor, const FVector& ContactPoint);

	UFUNCTION()
	virtual void OnDeathAnimationEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void FirstOverlappingItemPickUp();
	virtual void OnChangeTargetLockOnActor(AActor* NewTargetActor, bool bIsLockOn);

	uint8 bIsDefence : 1;
	uint8 bIsDead : 1;

public:	
	AVDCharacterBase();

	UFUNCTION()
	virtual void SetEquippedWeapon(AVDEquipItemVisualActor* NewWeapon);
	virtual void AddOverlappingItem(AVDItemPropActorBase* Item);
	virtual void RemoveOverlappingItem(AVDItemPropActorBase* Item);
	virtual bool PickItem(AVDItemPropActorBase* Item);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool UpdateEquippedItem(EVDEquipType EquipType, FName ItemID);
	virtual bool UseConsumeableItem(UVDInventoryInfo* Item); 
	FORCEINLINE UVDCharacterStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent; }
};
