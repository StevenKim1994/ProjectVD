// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VDCharacterBase.generated.h"

class AVDStagePlayerController;
class AVDWeapon;
UCLASS()
class PROJECTVD_API AVDCharacterBase : public ACharacter
{
	GENERATED_BODY()


protected:

	UPROPERTY()
	TObjectPtr<AVDStagePlayerController> CastPlayerController;

	virtual void BeginPlay() override;

public:	
	AVDCharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void EquipWeapon(AVDWeapon* NewWeapon);

};
