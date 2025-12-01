// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VDSequenceable.generated.h"

class ULevelSequence;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVDSequenceable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVD_API IVDSequenceable
{
	GENERATED_BODY()

public:
	virtual void OnSequenceStart() = 0;
	virtual void OnSequenceStop() = 0;
	virtual void OnSequencePause() = 0;
	virtual void OnSequenceResume() = 0;

	virtual ULevelSequence* GetSequence() const = 0;
};
