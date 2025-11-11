#pragma once

#include "CoreMinimal.h"

namespace VDConstants
{
	// 경로 리터럴
	constexpr const TCHAR* TitleMoviePath = TEXT("/Game/Movies/TitleMovie.TitleMovie");

#pragma region UI_WIDGET
	constexpr const TCHAR* TitlePanelWidgetPath = TEXT("/Game/UI/Title/WDG_TitlePanel.WDG_TitlePanel_C");

	extern const FName TitleWidgetButtonsParentsBox;
	extern const FName TitleStartButtonName;
	extern const FName TitleOptionButtonName;
	extern const FName TitleWidgetTitleName;
	extern const FName TitleExitButtonName;
	extern const FName TitleMovieSoundMuteToggleName;
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