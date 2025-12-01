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

	UPROPERTY()
	TWeakObjectPtr<AVDStagePlayerController> PlayerController;

	bool bIsInCutScene = false;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void StartCutScene(IVDSequenceable* CutSceneActor, FOnCutSceneFinishedDelegate OnFinishedDelegate = nullptr);
	FORCEINLINE bool IsInCutScene() const { return bIsInCutScene; }
};
