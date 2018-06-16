// Copyright 2018 by NiHongjian. All Rights Reserved.

using UnrealBuildTool;

public class LoginServer : ModuleRules
{
    public LoginServer(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePaths.Add("LoginServer/Privete");

        PrivateDependencyModuleNames.AddRange(
            new string[] {
				"Core",
                "CoreUObject",
                "Engine",
                "Sockets",
                "Networking",
			}
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {				
                "Networking"
			}
        );
	}
}
