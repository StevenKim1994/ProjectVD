// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VDISystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVDISystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVD_API IVDISystem
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

private:

protected:
	bool bIsInitialized = false;

	virtual void Initialize();
	virtual void Release();

public:
	virtual void LoadSystem() = 0;
	virtual void UnloadSystem() = 0;
};
