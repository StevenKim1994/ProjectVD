// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDCharacterBase.h"

// Sets default values
AVDCharacterBase::AVDCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

