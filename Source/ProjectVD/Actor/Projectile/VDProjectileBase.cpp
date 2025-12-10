// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/VDProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AVDProjectileBase::AVDProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AVDProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVDProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

