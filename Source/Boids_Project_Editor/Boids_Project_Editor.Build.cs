// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Boids_Project_Editor : ModuleRules
{
	public Boids_Project_Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Boids_Project", 
			"InputCore", "EnhancedInput", "GameplayTags", "Blutility", "UMG", "EditorScriptingUtilities", "EditorSubsystem",
			"MaterialEditor" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "AppFramework", "EditorStyle" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
	}
}
