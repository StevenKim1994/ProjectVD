// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VDUISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UVDUISubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
