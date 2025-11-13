// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectVDTarget : TargetRules
{
	public ProjectVDTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7; 

		// DESC :: 내가 추가한 모듈들
		ExtraModuleNames.Add("ProjectVD");
    }
}
