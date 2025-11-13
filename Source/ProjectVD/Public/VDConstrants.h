#pragma once

#include "CoreMinimal.h"

namespace VDConstants
{
#pragma region UI_REGISTRY
	// UI 레지스트리 데이터 에셋 경로
	extern const FString UIRegistryDataAssetPath;
#pragma endregion

#pragma region LEVEL_NAME
	// 로딩 레벨 이름
	extern const FName LoadingLevel;

#pragma region UI_WIDGET
	// UI 위젯 이름들
	extern const FName TitlePanel;
	extern const FName LoadingPanel;
#pragma endregion 
#pragma region UI_TEXT
	// 전역 프로젝트 이름 텍스트
	extern const FText TitleProjectName;

	// 버튼 텍스트 접근자 로컬라이즈 적용을 위한 함수들	
	const FText& GetTitleStartButtonText();
	const FText& GetTitleOptionButtonText();
	const FText& GetTitleExitButtonText();
	const FText& GetTitleSoundMuteToggleText();
#pragma endregion
}