// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VDCutSceneSubSystem.generated.h"

DECLARE_DELEGATE(FOnCutSceneFinishedDelegate);
UCLASS()
class PROJECTVD_API UVDCutSceneSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	bool bIsInCutScene = false;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void StartCutScene(AActor* CutSceneActor, FOnCutSceneFinishedDelegate OnFinishedDelegate = nullptr);
	FORCEINLINE bool IsInCutScene() const { return bIsInCutScene; }
};
