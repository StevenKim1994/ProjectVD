// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDEquipItemVisualActor.generated.h"

class UNiagaraComponent;

DECLARE_DELEGATE_SixParams(FOnDetectedHitColiderTarget, UPrimitiveComponent* , AActor* , UPrimitiveComponent* , int32 , bool , const FHitResult& );

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class PROJECTVD_API AVDEquipItemVisualActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<AActor*> DetectedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> EquipEffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="EquipItem", Meta = (AllowPrivateAccess ="true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem", Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;

	FOnDetectedHitColiderTarget OnDetectedHitColiderTarget;
protected:
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AVDEquipItemVisualActor();
	
	void SetColider(bool bEnable);

	FORCEINLINE FOnDetectedHitColiderTarget& GetOnDetectedHitColiderTarget() { return OnDetectedHitColiderTarget; }
};