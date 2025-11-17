// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/VDIPickable.h"
#include "Interface/VDPoolableInterface.h"
#include "VDWeapon.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UENUM()
enum class EWeaponType : uint8
{
	Sword,
};

UCLASS()
class PROJECTVD_API AVDWeapon : public AActor, public IVDIPickable, public IVDPoolableInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Weapon", Meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> HitBox;

public:	
	AVDWeapon();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual bool CanBePicked() override;
	virtual void OnPicked(AActor* Picker) override;

	virtual void OnPooledActivate() override;
	virtual void OnPooledDeactivate() override;
	virtual void ResetForReuse() override;
};
