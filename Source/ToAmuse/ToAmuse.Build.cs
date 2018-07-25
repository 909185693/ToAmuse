// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ToAmuse : ModuleRules
{
	public ToAmuse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "UMG",
            "Networking",
            "Engine",
            "InputCore",
            "GameplayTasks",
            "OnlineSubsystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "InputCore",
            "Slate",
            "SlateCore",
            "Json",
            "ApplicationCore"
        });

        PublicIncludePaths.AddRange(
            new string[] {
            }
        );
    }
}
