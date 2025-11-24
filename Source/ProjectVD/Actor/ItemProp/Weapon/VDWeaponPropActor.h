// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "VDWeaponPropActor.generated.h"

UCLASS()
class PROJECTVD_API AVDWeaponPropActor : public AVDItemPropActorBase
{
	GENERATED_BODY()
	
public:	
	AVDWeaponPropActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
};
