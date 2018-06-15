// Copyright 2018 by NiHongjian. All Rights Reserved.

using UnrealBuildTool;

public class LoginServer : ModuleRules
{
    public LoginServer(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePaths.Add("LoginServer/Private");
        
        PrivateDependencyModuleNames.AddRange(
            new string[] {
				"Core",
                "CoreUObject",
				"Sockets",
                "Networking",
                "Http",
                "Engine"
			}
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {				
                "Networking"
			}
        );
	}
}
