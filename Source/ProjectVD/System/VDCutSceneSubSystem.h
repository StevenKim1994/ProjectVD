// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VDCutSceneSubSystem.generated.h"

class AVDStagePlayerController;
class ULevelSequence;
class IVDSequenceable;

DECLARE_DELEGATE(FOnCutSceneFinishedDelegate);
UCLASS()
class PROJECTVD_API UVDCutSceneSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	FOnCutSceneFinishedDelegate OnCutSceneFinishedDelegate;

	UPROPERTY()
	TWeakObjectPtr<AVDStagePlayerController> PlayerController;

	UPROPERTY()
	TWeakObjectPtr<ULevelSequence> CurrentCutSceneSequence;

	TWeakInterfacePtr <IVDSequenceable> CurrentCutSceneActor;

	bool bIsInCutScene = false;

	UFUNCTION()
	void StartCutScene();

	UFUNCTION()
	void FinishCutScene();

	UFUNCTION()
	void CleanupCutScene();

	UFUNCTION()
	void PauseCutScene();

	UFUNCTION()
	void ResumeCutScene();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void StartCutScene(IVDSequenceable* CutSceneActor, FOnCutSceneFinishedDelegate OnFinishedDelegate = nullptr);
	FORCEINLINE bool IsInCutScene() const { return bIsInCutScene; }
};
