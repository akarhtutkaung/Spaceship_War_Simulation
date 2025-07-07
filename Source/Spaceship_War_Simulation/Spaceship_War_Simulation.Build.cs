// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Spaceship_War_Simulation : ModuleRules
{
	public Spaceship_War_Simulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
