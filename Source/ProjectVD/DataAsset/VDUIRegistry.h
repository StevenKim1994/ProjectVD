// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "VDUIRegistry.generated.h"

UCLASS()
class PROJECTVD_API UVDUIRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("UIRegistry", GetFName());
	}

	UFUNCTION(BlueprintCallable, Category = "UI")
	TSoftClassPtr<UUserWidget> GetWidgetClassByName(const FName& WidgetName) const;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& E) override;
#endif
private:
	// FName 키로 위젯 클래스를 매핑
	UPROPERTY(EditAnywhere, Category = "Widgets", meta = (AllowedClasses = "/Script/UMG.WidgetBlueprint"))
	TMap<FName, TSoftClassPtr<UUserWidget>> UIWidgetClasses;
};
