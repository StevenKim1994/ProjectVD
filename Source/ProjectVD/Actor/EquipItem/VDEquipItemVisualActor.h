// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDEquipItemVisualActor.generated.h"

class UNiagaraComponent;
DECLARE_DELEGATE_TwoParams(FOnDetectedHitColiderTarget, AActor* , const FVector& );

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class PROJECTVD_API AVDEquipItemVisualActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<AActor*> DetectedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> EquipEffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="EquipItem", Meta = (AllowPrivateAccess ="true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem", Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;

	FOnDetectedHitColiderTarget OnDetectedHitColiderTarget;
protected:

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AVDEquipItemVisualActor();
	
	void SetColider(bool bEnable);
	void SetDectedHitListReset();
	FORCEINLINE FOnDetectedHitColiderTarget& GetOnDetectedHitColiderTarget() { return OnDetectedHitColiderTarget; }
};