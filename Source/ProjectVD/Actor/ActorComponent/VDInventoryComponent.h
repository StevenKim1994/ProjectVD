// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDInventoryComponent.generated.h"

class UVDInventoryItem;
class AVDItemPropActorBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVD_API UVDInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVDInventoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION()
	bool IsInventoryFull() const;
		
	UFUNCTION()
	bool AddItemToInventory(AVDItemPropActorBase* Item);
};
