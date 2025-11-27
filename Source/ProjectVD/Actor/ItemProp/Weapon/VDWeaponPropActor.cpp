// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemProp/Weapon/VDWeaponPropActor.h"
#include "NiagaraComponent.h"

// Sets default values
AVDWeaponPropActor::AVDWeaponPropActor()
{
	PrimaryActorTick.bCanEverTick = true;
	EquipEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EquipEffectComp"));

	EquipEffectComp->SetupAttachment(RootComponent);

}

void AVDWeaponPropActor::BeginPlay()
{
	Super::BeginPlay();
}

void AVDWeaponPropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

