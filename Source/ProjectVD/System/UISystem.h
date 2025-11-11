// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/VDISystem.h"
#include "UISystem.generated.h"

UCLASS()
class PROJECTVD_API UUISystem : public UObject , public IVDISystem
{
	GENERATED_BODY()

private:

protected:
	void Initialize() override;
	void Release() override;

public:
	void LoadSystem() override;
	void UnloadSystem() override;
	
};
