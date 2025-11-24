// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AVDEquipItemVisualActor::AVDEquipItemVisualActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AVDEquipItemVisualActor::SetColider(bool bEnable)
{
	if (BoxComp)
	{
		BoxComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

void AVDEquipItemVisualActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVDEquipItemVisualActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

