using UnrealBuildTool;

public class EditorModule : ModuleRules
{
    public EditorModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UnrealEd", "AssetRegistry", "AssetTools"
        });
    }
}