// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "VDStagePlayerController.generated.h"

class UVDStagePlayerHUDWidget;
class UInputMappingContext;
class AVDEnemyCharacterBase;
class ACineCameraActor;

UCLASS()
class PROJECTVD_API AVDStagePlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TObjectPtr<AVDStagePlayerCharacter> VDStagePlayerCharacter;

	UPROPERTY()
	TObjectPtr<UVDStagePlayerHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CharacterControllerIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> UIControllerIMC;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> CurrentInputContext;

	void InitializeInputContext();

public:
	AVDStagePlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ShowBossStateBar(AVDEnemyCharacterBase* Boss);
	void HideBossStateBar();
	void ChangeToggleInputContext();
	void SetCutSceneCamera(ACineCameraActor* CineCamera); 
	void ClearCutSceneCamera();

	UFUNCTION() 
	void OnEscape(const FInputActionValue& Value);

	UFUNCTION()
	void OnInventory(const FInputActionValue& Value);

	FORCEINLINE TObjectPtr<UInputMappingContext> GetCharacterControllerIMC() const { return CharacterControllerIMC; }
};
