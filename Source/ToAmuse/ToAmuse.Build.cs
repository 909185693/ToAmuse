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
            "InputCore"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "LoginServer",
            "ClientNetwork"
        });

        PublicIncludePaths.AddRange(
            new string[] {
                "Common",
                "LoginServer/Public",
                "ClientNetwork/Public"
            }
        );
    }
}
