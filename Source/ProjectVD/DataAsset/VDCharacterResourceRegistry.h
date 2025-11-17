// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VDCharacterResourceRegistry.generated.h"

class UInputAction;
class UInputMappingContext;
UCLASS()
class PROJECTVD_API UVDCharacterResourceRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Character")
	TMap<FName, TSoftClassPtr<AActor>> CharacterClassMap;

	UPROPERTY(EditAnywhere, Category ="InputMappingContext")
	TMap<FName, TSoftObjectPtr<UInputMappingContext>> InputIMCClassMap;

};
