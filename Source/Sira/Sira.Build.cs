// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sira : ModuleRules
{
	public Sira(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities", 
			"GameplayAbilities", 
			"GameplayAbilities", 
			"AnimGraphRuntime", 
			"UMG", 
			"AIModule", 
			"GameplayTags", 
			"GameplayTags",
			"NavigationSystem",
			"ProceduralMeshComponent",
			"PoissonPoints",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}