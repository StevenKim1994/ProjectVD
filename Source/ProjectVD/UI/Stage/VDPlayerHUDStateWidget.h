// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDPlayerHUDStateWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UVDCharacterStatsBaseComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSetCharacterState, float, float);

UCLASS()
class PROJECTVD_API UVDPlayerHUDStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void ShowPerformanceTween();
	void SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats);

	void SetHPBarPercent(float CurrentHP, float MaxHP);
	void SetMPBarPercent(float CurrentMP, float MaxMP);

	FORCEINLINE FOnSetCharacterState& GetOnSetCharacterStateDelegate() { return OnSetCharacterStateDelegate; }

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPBarText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> MPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MPBarText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

	FOnSetCharacterState OnSetCharacterStateDelegate;

	float TargetHPPercent;
	bool bIsHPTweenPlaying;

	float TargetMPPercent;
	bool bIsMPTweenPlaying;

};
