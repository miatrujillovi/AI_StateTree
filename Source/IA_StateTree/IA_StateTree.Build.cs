// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IA_StateTree : ModuleRules
{
	public IA_StateTree(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"GameplayTasks",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"StructUtils",
			"UMG",
			"Slate"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"StateTreeEditorModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"IA_StateTree",
			"IA_StateTree/Variant_Platforming",
			"IA_StateTree/Variant_Platforming/Animation",
			"IA_StateTree/Variant_Combat",
			"IA_StateTree/Variant_Combat/AI",
			"IA_StateTree/Variant_Combat/Animation",
			"IA_StateTree/Variant_Combat/Gameplay",
			"IA_StateTree/Variant_Combat/Interfaces",
			"IA_StateTree/Variant_Combat/UI",
			"IA_StateTree/Variant_SideScrolling",
			"IA_StateTree/Variant_SideScrolling/AI",
			"IA_StateTree/Variant_SideScrolling/Gameplay",
			"IA_StateTree/Variant_SideScrolling/Interfaces",
			"IA_StateTree/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
