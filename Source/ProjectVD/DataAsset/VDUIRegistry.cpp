// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/VDUIRegistry.h"

#if WITH_EDITOR
static FName MakeKeyFromPath(const FSoftObjectPath& Path)
{
	// ex) /Game/UI/WBP_MainMenu.WBP_MainMenu_C -> WBP_MainMenu
	const FString AssetName = Path.GetAssetName(); // WBP_MainMenu_C
	FString Base = AssetName;
	Base.RemoveFromEnd(TEXT("_C"));
	return FName(*Base);
}

TSoftClassPtr<UUserWidget> UVDUIRegistry::GetWidgetClassByName(const FName& WidgetName) const
{
	if (const TSoftClassPtr<UUserWidget>* FoundClass = UIWidgetClasses.Find(WidgetName))
	{
		return *FoundClass;
	}
	return nullptr;
}

void UVDUIRegistry::PostEditChangeProperty(FPropertyChangedEvent& E)
{
	Super::PostEditChangeProperty(E);

	// 맵 변경 시 자동 키 채우기 & 경로 정규화
	if (!E.Property) return;
	
	const FName PropName = E.Property->GetFName();
	if (PropName != GET_MEMBER_NAME_CHECKED(UVDUIRegistry, UIWidgetClasses)) return;

	// 맵 전체를 한번 스캔해, 값이 채워진 항목 중 키가 None이거나 빈 경우 자동 보정
	TMap<FName, TSoftClassPtr<UUserWidget>> Fixed;
	Fixed.Reserve(UIWidgetClasses.Num());

	for (auto& It : UIWidgetClasses)
	{
		FName Key = It.Key;
		TSoftClassPtr<UUserWidget> Value = It.Value;
		
		if (Value.IsNull())
		{
			// 값이 비어있으면 그대로 유지 (에디터에서 사용자가 UMG 위젯을 선택할 수 있음)
			Fixed.Add(Key, Value);
			continue;
		}

		// TSoftClassPtr은 자동으로 UUserWidget 클래스로 제한됨
		// 블루프린트 위젯의 경우 _C 경로가 필요
		FSoftObjectPath P = Value.ToSoftObjectPath();
		FString ObjPath = P.ToString();
		
		// Blueprint 클래스인 경우 _C suffix 확인
		if (!ObjPath.IsEmpty() && !ObjPath.EndsWith(TEXT("_C")))
		{
			// /Game/UI/WBP_MainMenu.WBP_MainMenu 형식에서
			// 패키지 경로와 애셋 이름을 분리
			const FString PackageName = P.GetLongPackageName(); // /Game/UI/WBP_MainMenu
			const FString AssetName = P.GetAssetName(); // WBP_MainMenu
			const FString SubObjectName = P.GetSubPathString(); // WBP_MainMenu 또는 빈 문자열
			
			// SubObject가 없거나 _C로 끝나지 않는 경우
			if (SubObjectName.IsEmpty())
			{
				// /Game/UI/WBP_MainMenu.WBP_MainMenu_C 형식으로 재구성
				ObjPath = FString::Printf(TEXT("%s.%s_C"), *PackageName, *AssetName);
				Value = TSoftClassPtr<UUserWidget>(FSoftObjectPath(ObjPath));
			}
			else if (!SubObjectName.EndsWith(TEXT("_C")))
			{
				// SubObject가 있지만 _C가 없는 경우
				ObjPath = FString::Printf(TEXT("%s.%s_C"), *PackageName, *SubObjectName);
				Value = TSoftClassPtr<UUserWidget>(FSoftObjectPath(ObjPath));
			}
		}

		// 키가 비어있으면 경로에서 자동 생성
		if (Key.IsNone() || Key.IsEqual(NAME_None) || Key.ToString().TrimStartAndEnd().IsEmpty())
		{
			Key = MakeKeyFromPath(Value.ToSoftObjectPath());
		}

		// 키 중복 방지: 충돌 시 뒤에 숫자 붙이기
		FName FinalKey = Key;
		int32 Suffix = 1;
		while (Fixed.Contains(FinalKey))
		{
			FinalKey = FName(*(Key.ToString() + FString::Printf(TEXT("_%d"), Suffix++)));
		}

		Fixed.Add(FinalKey, Value);
	}



	UIWidgetClasses = MoveTemp(Fixed);
	MarkPackageDirty();
}
#endif
