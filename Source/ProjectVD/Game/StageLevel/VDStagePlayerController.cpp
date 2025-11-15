#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"
AVDStagePlayerController::AVDStagePlayerController()
{
}

void AVDStagePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	if (UVDUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UVDUISubsystem>())
	{
		UISubsystem->SetPlayerControllerRootUIWidget(this);
		UISubsystem->SetCurrentHUDWidget(VDConstants::StagePlayerHUD);
	}
}
