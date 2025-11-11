// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "System/UISystem.h"
#include "VDGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDGameInstance : public UGameInstance
{
	GENERATED_BODY()

#pragma region LEVEL MANAGE

public:
	void Init() override;
	void GotoInGameLevel(FName _TargetLevelName);
	bool DoesLevelExist(FName _TargetLevelName);
	void OnPostLoadLevel();

protected:
	UUISystem* UISystem;

private:
	FName TargetLevelName;
#pragma endregion 

};
