// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VDPoolableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVDPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVD_API IVDPoolableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void OnPooledActivate() = 0;      
    virtual void OnPooledDeactivate() = 0;    
    virtual void ResetForReuse() = 0;         
};
