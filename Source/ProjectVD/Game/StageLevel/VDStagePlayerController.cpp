#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "System/VDUISubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Public/VDConstrants.h"
#include "UI/Stage/VDStagePauseWidget.h"
#include "Game/VDGameInstance.h"
#include "System/VDLevelSystem.h"

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
				if (ActionName.StartsWith(TEXT("UI_IA_")))
				{
					ActionName = ActionName.RightChop(6);
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
			// DefaultMappingContext 비활성화
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
			if (Subsystem && DefaultMappingContext)
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
			}

			UISubsystem->ShowUIWidgetAsync(VDConstants::PauseMenuWidget, FOnUIWidgetLoadedDelegate::CreateWeakLambda(this, [this](UUserWidget* LoadWidget)
				{
					if (LoadWidget)
					{
						UVDStagePauseWidget* PauseWidget = Cast<UVDStagePauseWidget>(LoadWidget);
						if (PauseWidget)
						{
							PauseWidget->OnPauseMenuButtonClickedDelegate.BindLambda([this](PauseMenuButtonEnum ClickedButton)
								{
									if (ClickedButton == PauseMenuButtonEnum::Resume)
									{
										if (UVDUISubsystem* UISubsystemInner = GetGameInstance()->GetSubsystem<UVDUISubsystem>())
										{
											UISubsystemInner->PopModalUIWidget();
											bShowMouseCursor = false;

											// DefaultMappingContext 다시 활성화
											UEnhancedInputLocalPlayerSubsystem* Subsystem =
												ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
											if (Subsystem && DefaultMappingContext)
											{
												Subsystem->AddMappingContext(DefaultMappingContext, 0);
											}
										}
									}
									else if (ClickedButton == PauseMenuButtonEnum::Options)
									{
										// TODO :: 옵션 메뉴 처리
									}
									else if (ClickedButton == PauseMenuButtonEnum::ExitToTitle)
									{
										// TODO :: 타이틀 화면으로 돌아가기 처리
										UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();
										if (GI)
										{
											if (UVDLevelSystem* LevelSystem = GI->GetSubsystem<UVDLevelSystem>())
											{
												LevelSystem->ChangeLevelByName("Title");
											}
										}
									}
								});

							bShowMouseCursor = true;
						}
					}
				})
			);
		}
	}
}
