// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "ActorPool.generated.h"

/**
 * 
 */
USTRUCT()
struct PROJECTVD_API FActorPool 
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Inactive;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> Active;

	int32 Capacity = 32;
	int32 DesirePrewarm = 0;
	bool bAutoExpand = true;

	int32 TotalSpawned = 0;
};
