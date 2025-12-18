// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectVD : ModuleRules
{
	public ProjectVD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ProjectVD" });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "UMG", "MediaAssets", "AudioMixer",
            "EnhancedInput", "NavigationSystem", "AIModule" , "Niagara", "NiagaraAnimNotifies", "CinematicCamera", 
            "LevelSequence",  "MovieScene", "MovieSceneTracks", "AnimGraphRuntime", "GameplayCameras", "GameplayAbilities", "GameplayTags", "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
