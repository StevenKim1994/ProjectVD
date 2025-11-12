// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "VDUIRegistry.generated.h"

/**
 * UI 위젯 레퍼런스를 중앙 집중식으로 관리하는 DataAsset
 * Unity Addressable과 유사한 키 기반 UI 관리 시스템
 */
UCLASS()
class PROJECTVD_API UVDUIRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("UIRegistry", GetFName());
	}

	/**
	 * FName 키로 위젯 클래스를 가져옵니다
	 * @param WidgetKey - 위젯을 식별하는 키
	 * @return 위젯 클래스의 소프트 레퍼런스
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	TSoftClassPtr<UUserWidget> GetWidgetClass(FName WidgetKey) const
	{
		if (const TSoftClassPtr<UUserWidget>* FoundClass = UIWidgetClasses.Find(WidgetKey))
		{
			return *FoundClass;
		}
		return nullptr;
	}

	/**
	 * 템플릿 기반으로 타입 안전한 위젯 클래스 가져오기
	 * @param WidgetKey - 위젯을 식별하는 키
	 * @return 지정된 타입의 위젯 클래스 소프트 레퍼런스
	 */
	template<typename T>
	TSoftClassPtr<T> GetWidgetClassTyped(FName WidgetKey) const
	{
		static_assert(TIsDerivedFrom<T, UUserWidget>::IsDerived, "T must be derived from UUserWidget");
		
		if (const TSoftClassPtr<UUserWidget>* FoundClass = UIWidgetClasses.Find(WidgetKey))
		{
			return TSoftClassPtr<T>(FoundClass->ToSoftObjectPath());
		}
		return nullptr;
	}

	/**
	 * 위젯 클래스를 동적으로 등록합니다
	 * @param WidgetKey - 위젯을 식별하는 키
	 * @param WidgetClass - 등록할 위젯 클래스
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RegisterWidgetClass(FName WidgetKey, TSoftClassPtr<UUserWidget> WidgetClass)
	{
		UIWidgetClasses.Add(WidgetKey, WidgetClass);
	}

	/**
	 * 등록된 위젯 클래스를 제거합니다
	 * @param WidgetKey - 제거할 위젯의 키
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UnregisterWidgetClass(FName WidgetKey)
	{
		UIWidgetClasses.Remove(WidgetKey);
	}

	/**
	 * 특정 키가 등록되어 있는지 확인합니다
	 * @param WidgetKey - 확인할 위젯의 키
	 * @return 등록 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool HasWidgetClass(FName WidgetKey) const
	{
		return UIWidgetClasses.Contains(WidgetKey);
	}

	/**
	 * 등록된 모든 위젯 키를 반환합니다
	 * @return 등록된 모든 키의 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	TArray<FName> GetAllWidgetKeys() const
	{
		TArray<FName> Keys;
		UIWidgetClasses.GetKeys(Keys);
		return Keys;
	}

private:
	// FName 키로 위젯 클래스를 매핑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TMap<FName, TSoftClassPtr<UUserWidget>> UIWidgetClasses;
};
