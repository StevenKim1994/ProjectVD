// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "VDWeaponPropActor.generated.h"

class UNagaraComponent;

UCLASS()
class PROJECTVD_API AVDWeaponPropActor : public AVDItemPropActorBase
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProp", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> EquipEffectComp;

public:	
	AVDWeaponPropActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
};
