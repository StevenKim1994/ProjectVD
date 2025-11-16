#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "System/VDUISubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
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

void AVDStagePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem && DefaultMappingContext)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		const TArray<FEnhancedActionKeyMapping>& Mappings = DefaultMappingContext->GetMappings();
		for (const FEnhancedActionKeyMapping& Mapping : Mappings)
		{
			const UInputAction* Action = Mapping.Action;
			if (Action)
			{
				FString ActionName = Action->GetName();
				if (ActionName.StartsWith(TEXT("UI_IA")))
				{
					ActionName = ActionName.RightChop(4); 
					EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, FName(ActionName));
				}
			}
		}
	}
}

void AVDStagePlayerController::OnEscape(const FInputActionValue& Value)
{
	if (UVDUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UVDUISubsystem>())
	{
		if (UISubsystem->GetModalUIWidgetCount() > 0)
		{
			UISubsystem->PopModalUIWidget();
		}
		else
		{
			// TODO :: 타임스케일 조정 등 게임 일시정지 처리
			UISubsystem->ShowUIWidgetAsync(VDConstants::PauseMenuWidget);
		}
	}
}
