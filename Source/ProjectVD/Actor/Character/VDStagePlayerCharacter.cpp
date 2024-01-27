// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDStagePlayerCharacter.h"

// Sets default values
AVDStagePlayerCharacter::AVDStagePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CharacterClass = EClassType::None;
}

// Called when the game starts or when spawned
void AVDStagePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDStagePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVDStagePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

