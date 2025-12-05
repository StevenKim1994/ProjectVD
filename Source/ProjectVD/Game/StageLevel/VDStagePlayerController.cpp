#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/Character/VDCharacterBase.h"
#include "System/VDUISubsystem.h"
#include "System/VDPlayerSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Public/VDConstrants.h"
#include "UI/Stage/VDStagePlayerHUDWidget.h"
#include "UI/Stage/VDStagePauseWidget.h"
#include "UI/Stage/Inventory/VDInventoryPanel.h"
#include "Game/VDGameInstance.h"
#include "System/VDLevelSystem.h"
#include "CineCameraActor.h"	
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"
#include "Game/StageLevel/VDStageGameMode.h"
	
AVDStagePlayerController::AVDStagePlayerController()
{
}

void AVDStagePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	
}

void AVDStagePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InitializeInputContext();
}

void AVDStagePlayerController::InitializeInputContext()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		if (CharacterControllerIMC)
		{
			const TArray<FEnhancedActionKeyMapping>& Mappings = CharacterControllerIMC->GetMappings();
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

		if (UIControllerIMC)
		{
			const TArray<FEnhancedActionKeyMapping>& Mappings = UIControllerIMC->GetMappings();
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

	// DESC :: 기본 캐릭터 인풋컨텍스트
	Subsystem->AddMappingContext(CharacterControllerIMC, 0);
	CurrentInputContext = CharacterControllerIMC;
}

void AVDStagePlayerController::ChangeToggleInputContext()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		if (Subsystem->HasMappingContext(CharacterControllerIMC)) // DESC :: 현재 캐릭터 인풋컨텍스트일떄
		{
			Subsystem->RemoveMappingContext(CharacterControllerIMC);
			Subsystem->AddMappingContext(UIControllerIMC, 0);
			CurrentInputContext = UIControllerIMC;
			bShowMouseCursor = false;
			return;
		}
		else if(Subsystem->HasMappingContext(UIControllerIMC))// DESC :: 현재 UI 인풋컨텍스트일떄
		{
			Subsystem->RemoveMappingContext(UIControllerIMC);
			Subsystem->AddMappingContext(CharacterControllerIMC, 0);
			CurrentInputContext = CharacterControllerIMC;
			bShowMouseCursor = true;
			return;
		}
	}
}

void AVDStagePlayerController::SetCutSceneCamera(ACineCameraActor* CineCamera)
{
	if (CineCamera)
	{
		// DESC :: 컷신 시작 - 입력 비활성화

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (Subsystem->HasMappingContext(CharacterControllerIMC))
			{
				Subsystem->RemoveMappingContext(CharacterControllerIMC);
			}
			if (Subsystem->HasMappingContext(UIControllerIMC))
			{
				Subsystem->RemoveMappingContext(UIControllerIMC);
			}
		}

		AActor* CineCameraActor = Cast<AActor>(CineCamera);
		SetViewTargetWithBlend(CineCameraActor, 0.1f, EViewTargetBlendFunction::VTBlend_Cubic);

		UGameInstance* GameInstance = GetGameInstance();
		if (GameInstance)
		{
			if (UVDUISubsystem* UISubsystem = GameInstance->GetSubsystem<UVDUISubsystem>())
			{
				UISubsystem->HideCurrentHUDWidget();
			}
		}
	}
}

void AVDStagePlayerController::ShakePlayerHitCameraEffect(float Scale)
{
	if (PlayerCameraManager)
	{
		if (CameraShakeEffect)
		{
			PlayerCameraManager->StartCameraShake(CameraShakeEffect, Scale);
		}
	}
}

void AVDStagePlayerController::SetGameOver()
{
	SetIgnoreMoveInput(true);
	if (UVDUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UVDUISubsystem>())
	{
		UISubsystem->ShowUIWidgetAsync(TEXT("StageGameOverWidget"));

		AVDStageGameMode* GM = Cast<AVDStageGameMode>(UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->OnPlayerDeath();
		}
	}
}

void AVDStagePlayerController::SetCharacter(AVDCharacterBase* NewCharacter)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (UVDUISubsystem* UISubsystem = GameInstance->GetSubsystem<UVDUISubsystem>())
	{
		UISubsystem->SetPlayerControllerRootUIWidget(this);
		UISubsystem->SetCurrentHUDWidget(VDConstants::StagePlayerHUD);

		HUDWidget = Cast<UVDStagePlayerHUDWidget>(UISubsystem->GetCurrentHUDWidget());
		if (HUDWidget)
		{
			HUDWidget->SetCharacter(NewCharacter);
		}
	}

	if (UVDPlayerSubsystem* PlayerSubsytem = GameInstance->GetSubsystem<UVDPlayerSubsystem>())
	{
		PlayerSubsytem->SetCurrentCharacter(NewCharacter);
	}
}

void AVDStagePlayerController::ClearCutSceneCamera()
{
	SetViewTargetWithBlend(GetPawn(), 0.3f, EViewTargetBlendFunction::VTBlend_Cubic);

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		if (UVDUISubsystem* UISubsystem = GameInstance->GetSubsystem<UVDUISubsystem>())
		{
			UISubsystem->ShowCurrentHUDWidget();
		}
	}

	// DESC :: 컷신 종료 - 입력 재활성화
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CurrentInputContext, 0);
	}
}

void AVDStagePlayerController::ShowInteractionWidget(const FText& InText, const FText& InKeyText)
{
	if (HUDWidget)
	{
		HUDWidget->ShowInteractionWidget(InText, InKeyText);
	}
}

void AVDStagePlayerController::HideInteractionWidget()
{
	if(HUDWidget)
	{
		HUDWidget->HideInteractionWidget();
	}
}

void AVDStagePlayerController::OnEscape(const FInputActionValue& Value)
{

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem && Subsystem->HasMappingContext(CharacterControllerIMC)) // DESC :: 현재 캐릭터 인풋컨텍스트일떄
	{
		if (UVDUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UVDUISubsystem>())
		{
			if (!UISubsystem->IsModalUIWidgetStackEmpty())
			{
				UISubsystem->PopModalUIWidget();
			}
			else
			{
				ChangeToggleInputContext();
				UGameplayStatics::SetGamePaused(this, true);

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

												UGameplayStatics::SetGamePaused(this, false);
												ChangeToggleInputContext();
												UISubsystemInner->ShowCurrentHUDWidget();
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
												GI->GetSubsystem<UVDUISubsystem>()->AllModalUIWidgetClear();
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
}

void AVDStagePlayerController::OnInventory(const FInputActionValue& Value)
{
	if (UVDUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UVDUISubsystem>())
	{
		if (UISubsystem->IsModalUIWidgetStackEmpty())
		{
			UISubsystem->ShowUIWidgetAsync(VDConstants::InventoryPanelWidget);
			ChangeToggleInputContext();
			bShowMouseCursor = true;
		}
		else
		{
			if (UUserWidget* InvenWidget = UISubsystem->PeekModalUIWidget())
			{
				if (InvenWidget)
				{
					UISubsystem->HideUIWidget(VDConstants::InventoryPanelWidget);
					ChangeToggleInputContext();
					bShowMouseCursor = false;
				}
			}
		}
	}
}

void AVDStagePlayerController::ShowBossStateBar(AVDEnemyCharacterBase* Boss)
{
	if (HUDWidget)
	{
		HUDWidget->ShowBossStatus(Boss);
	}
}

void AVDStagePlayerController::HideBossStateBar()
{
	if(HUDWidget)
	{
		HUDWidget->HideBossStatus();
	}
}
