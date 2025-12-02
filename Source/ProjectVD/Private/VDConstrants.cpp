#include "VDConstrants.h"
#include "Internationalization/Text.h"

namespace VDConstants
{
	const FName StagePlayerHUD(TEXT("StagePlayerHUD"));
#pragma region UI_WIDGET
	const FName TitlePanel(TEXT("TitlePanel"));
	const FName CutSceneNamePlateWidget(TEXT("BossNamePlate"));
	const FName LoadingPanel(TEXT("LoadingPanel"));
	const FName RootUIWidget(TEXT("RootUIWidget"));
	const FName ToastContainer(TEXT("ToastContainer"));
	const FName PauseMenuWidget(TEXT("PauseMenuWidget"));
	const FName InventoryPanelWidget(TEXT("InventoryPanel"));
	const FName ConfirmPopupWidget(TEXT("ConfirmPopupWidget"));
#pragma endregion
	const FName TitleWidgetTitleName(TEXT("GameTitleName"));
	const FName TitleWidgetButtonsParentsBox(TEXT("ButtonsParentBox"));
	const FName TitleStartButtonName(TEXT("StartButton"));
	const FName TitleOptionButtonName(TEXT("OptionButton"));
	const FName TitleExitButtonName(TEXT("ExitButton"));
	const FName TitleMovieSoundMuteToggleName(TEXT("TitleMovieSoundMuteToggle"));

	const FString UIRegistryDataAssetPath(TEXT("/Game/ProjectVD/DataAsset/UIRegistry.UIRegistry"));
	const FString TableRegistryDataAssetPath(TEXT("/Game/ProjectVD/DataAsset/TableRegistry.TableRegistry"));
	const FName LoadingLevel(TEXT("Loading"));


	// 로컬라이제이션 가능한 프로젝트 이름
	const FText TitleProjectName = NSLOCTEXT("VD.UI", "ProjectName", "ProjectVD");

	const FText& GetTitleStartButtonText()
	{
		static const FText Text = NSLOCTEXT("VD.UI", "TitleStartButton", "START");
		return Text;
	}

	const FText& GetTitleOptionButtonText()
	{
		static const FText Text = NSLOCTEXT("VD.UI", "TitleOptionButton", "OPTION");
		return Text;
	}

	const FText& GetTitleExitButtonText()
	{
		static const FText Text = NSLOCTEXT("VD.UI", "TitleExitButton", "EXIT");
		return Text;
	}
	const FText& GetTitleSoundMuteToggleText()
	{
		static const FText Text = NSLOCTEXT("VD.UI", "TitleSoundMuteToggle", "MUTE");
		return Text;
	}
	const FText& GetToastItemPickupText()
	{
		static const FText Text = NSLOCTEXT("VD.UI", "ToastItemPickup", "아이템 획득");
		return Text;
	}

	const FText& GetToastInventoryIsFullText()
	{
		static const FText Text = NSLOCTEXT("VD.UI", "ToastInventoryIsFull", "인벤토리가 가득 찼습니다.");
		return Text;
	}

	const FText& GetInventoryEquipButtonText()
	{
		static const FText Text = NSLOCTEXT("VD.UI.Inventory", "EquipButton", "장착");
		return Text;
	}

	const FText& GetInventoryUseButtonText()
	{
		static const FText Text = NSLOCTEXT("VD.UI.Inventory", "UseButton", "사용");
		return Text;
	}
}