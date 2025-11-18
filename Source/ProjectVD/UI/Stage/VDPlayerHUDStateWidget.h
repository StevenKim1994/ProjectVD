// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDPlayerHUDStateWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UVDCharacterStatsBaseComponent;

DECLARE_DELEGATE_OneParam(FOnSetCharacterState, UVDCharacterStatsBaseComponent*);
UCLASS()
class PROJECTVD_API UVDPlayerHUDStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
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

public:
	UFUNCTION()
	void SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats);

	FOnSetCharacterState& GetOnSetCharacterStateDelegate() { return OnSetCharacterStateDelegate; }
};
