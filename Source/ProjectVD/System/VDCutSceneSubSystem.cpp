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

void UVDCutSceneSubSystem::StartCutScene(IVDSequenceable* CutSceneActor, FOnCutSceneFinishedDelegate OnFinishedDelegate)
{
	bIsInCutScene = true;

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


	// 바인딩 구조 획득
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

	PlayerController->SetCutSceneCamera(CameraActor);

	SequencePlayer->Play();

	if (OnFinishedDelegate.IsBound())
	{
		OnFinishedDelegate.Execute();
	}
	bIsInCutScene = false;
}
