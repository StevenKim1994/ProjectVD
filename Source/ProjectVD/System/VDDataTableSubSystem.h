// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Engine/DataTable.h" 
#include "VDDataTableSubSystem.generated.h"

UCLASS()
class PROJECTVD_API UVDDataTableSubSystem : public USubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FName, TSoftObjectPtr<UDataTable>> RegisteredDataTables;

	UPROPERTY()
	TMap<FName, TObjectPtr<UDataTable>> LoadedDataTables;	

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void RegisterTable(FName TableName, UDataTable* DataTable);
};
