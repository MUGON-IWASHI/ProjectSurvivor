// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectSurvivor : ModuleRules
{
	public ProjectSurvivor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProjectSurvivor",
			"ProjectSurvivor/Variant_Platforming",
			"ProjectSurvivor/Variant_Platforming/Animation",
			"ProjectSurvivor/Variant_Combat",
			"ProjectSurvivor/Variant_Combat/AI",
			"ProjectSurvivor/Variant_Combat/Animation",
			"ProjectSurvivor/Variant_Combat/Gameplay",
			"ProjectSurvivor/Variant_Combat/Interfaces",
			"ProjectSurvivor/Variant_Combat/UI",
			"ProjectSurvivor/Variant_SideScrolling",
			"ProjectSurvivor/Variant_SideScrolling/AI",
			"ProjectSurvivor/Variant_SideScrolling/Gameplay",
			"ProjectSurvivor/Variant_SideScrolling/Interfaces",
			"ProjectSurvivor/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
