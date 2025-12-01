// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDCutSceneSubSystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interface/VDSequenceable.h"
#include "MovieScene.h"
#include "MovieSceneBinding.h"
#include "MovieSceneSequencePlayer.h"
#include "MovieSceneTrack.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CineCameraActor.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "System/VDUISubsystem.h"

void UVDCutSceneSubSystem::StartCutScene()
{
	if (CurrentCutSceneActor.IsValid() && CurrentCutSceneSequence.IsValid())
	{

		bIsInCutScene = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("CutScene Started"));
}

void UVDCutSceneSubSystem::FinishCutScene()
{
	if (CurrentCutSceneActor.IsValid() && CurrentCutSceneSequence.IsValid())
	{
		bIsInCutScene = false;
		if (OnCutSceneFinishedDelegate.IsBound())
		{
			OnCutSceneFinishedDelegate.Execute();
		}
	}
	PlayerController->ClearCutSceneCamera();
	UE_LOG(LogTemp, Warning, TEXT("CutScene Finished"));
}

void UVDCutSceneSubSystem::CleanupCutScene()
{
	if (CurrentCutSceneActor.IsValid() && CurrentCutSceneSequence.IsValid())
	{
		bIsInCutScene = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("CutScene Cleaned up"));
}

void UVDCutSceneSubSystem::PauseCutScene()
{
	if (CurrentCutSceneActor.IsValid() && CurrentCutSceneSequence.IsValid())
	{

	}

	UE_LOG(LogTemp, Warning, TEXT("CutScene Paused"));
}

void UVDCutSceneSubSystem::ResumeCutScene()
{
	if (CurrentCutSceneActor.IsValid() && CurrentCutSceneSequence.IsValid())
	{

	}

	UE_LOG(LogTemp, Warning, TEXT("CutScene Resumed"));
}

void UVDCutSceneSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	PlayerController = GetWorld()->GetFirstPlayerController<AVDStagePlayerController>();
}

void UVDCutSceneSubSystem::OnWorldEndPlay(UWorld& InWorld)
{
	Super::OnWorldEndPlay(InWorld);
}

void UVDCutSceneSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDCutSceneSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UVDCutSceneSubSystem::ShowNamePlate(const FName& InName)
{
	UVDUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UVDUISubsystem>();
	if (UISubsystem)
	{
		//UISubsystem->ShowNamePlate(InName);
		UE_LOG(LogTemp, Warning, TEXT("Show Name Plate: %s"), InName);
	}
}

void UVDCutSceneSubSystem::HideNamePlate()
{
	UVDUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UVDUISubsystem>();
	if (UISubsystem)
	{
		//UISubsystem->HideNamePlate();
		UE_LOG(LogTemp, Warning, TEXT("Hide Name Plate"));
	}
}

void UVDCutSceneSubSystem::StartCutScene(IVDSequenceable* CutSceneActor, FOnCutSceneFinishedDelegate OnFinishedDelegate)
{
	if(bIsInCutScene) // DESC :: 이미 컷씬 진행이면 무시함.
	{
		return;
	}

	FMovieSceneSequencePlaybackSettings Settings;
	ALevelSequenceActor* OutActor = nullptr;
	ULevelSequence* LevelSequence = CutSceneActor->GetSequence();
	if (LevelSequence == nullptr)
	{
		return;
	}

	UMovieScene* MovieScene = LevelSequence->GetMovieScene();
	if (MovieScene == nullptr)
	{
		return;
	}

	OnCutSceneFinishedDelegate = OnFinishedDelegate;
	CurrentCutSceneActor = CutSceneActor;
	CurrentCutSceneSequence = LevelSequence;

	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequence,
		Settings,
		OutActor);

	if (SequencePlayer == nullptr || OutActor == nullptr)
	{
		return;
	}

	OutActor->SetBindingByTag(FName(TEXT("Boss")), TArray<AActor*>{ Cast<AActor>(CutSceneActor) });
	FMovieSceneObjectBindingID CameraID = OutActor->FindNamedBinding(FName(TEXT("Camera")));

	FMovieSceneBinding* CameraBinding = MovieScene->FindBinding(CameraID.GetGuid());
	if (CameraBinding == nullptr)
	{
		return;
	}
	const FGuid CameraGuid = CameraID.GetGuid();

	// DESC :: 카메라는 Spawnable이므로 프레임을 시작으로 옮긴다음 카메라정보를 얻어옴.
	const TRange<FFrameNumber> PlaybackRange = MovieScene->GetPlaybackRange();
	FFrameNumber StartFrame = PlaybackRange.GetLowerBound().IsClosed() ? PlaybackRange.GetLowerBoundValue() : FFrameNumber(0);
	FMovieSceneSequencePlaybackParams Params;
	Params.Frame = StartFrame;
	Params.PositionType = EMovieScenePositionType::Frame;
	Params.UpdateMethod = EUpdatePositionMethod::Jump;
	SequencePlayer->SetPlaybackPosition(Params);

	ACineCameraActor* CameraActor = nullptr;
	TArray<UObject*> BoundObjects;

	BoundObjects = SequencePlayer->GetBoundObjects(CameraID);
	for (UObject* BoundObject : BoundObjects)
	{
		if (ACineCameraActor* FoundCamera = Cast<ACineCameraActor>(BoundObject))
		{
			CameraActor = FoundCamera;
			break;
		}
	}

	CutSceneActor->OnSequenceStart();
	PlayerController->SetCutSceneCamera(CameraActor);

	SequencePlayer->OnFinished.AddDynamic(this, &UVDCutSceneSubSystem::FinishCutScene);
	SequencePlayer->OnPause.AddDynamic(this, &UVDCutSceneSubSystem::PauseCutScene);
	SequencePlayer->OnStop.AddDynamic(this, &UVDCutSceneSubSystem::CleanupCutScene);
	//SequencePlayer->ProcessEvent(this->FindFunction(TEXT("ShowNamePlate")), "TEST");
	// 컷신에서 특정 메서드 호출방법 연구하기
	SequencePlayer->Play();
	StartCutScene();
	bIsInCutScene = true;
}
