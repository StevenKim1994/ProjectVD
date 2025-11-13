// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VDGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;

	void SetFrameRate(int32 NewFrameRate);

};
