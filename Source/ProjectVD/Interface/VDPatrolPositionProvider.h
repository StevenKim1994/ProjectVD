// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VDPatrolPositionProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVDPatrolPositionProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVD_API IVDPatrolPositionProvider
{
	GENERATED_BODY()

public:
	virtual TArray<FVector> GetPatrolPosition() const = 0;
};
