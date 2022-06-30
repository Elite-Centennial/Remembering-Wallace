// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RememberingWallace : ModuleRules
{
	public RememberingWallace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"DeveloperSettings",
		});
	}
}
