// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WarKing : ModuleRules
{
	public WarKing(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "WarKing" });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "MultiplayerSessions",
            "EnhancedInput", "OnlineSubSystemSteam", "OnlineSubSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "GameFeatures",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
