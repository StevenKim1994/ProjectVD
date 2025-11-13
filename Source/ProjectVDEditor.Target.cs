// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectVDEditorTarget : TargetRules
{
	public ProjectVDEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;



        // DESC :: 내가 추가한 모듈들
        ExtraModuleNames.Add("ProjectVD");
	}
}
