// Copyright 2018 by NiHongjian. All Rights Reserved.

using UnrealBuildTool;

public class ClientNetwork : ModuleRules
{
    public ClientNetwork(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePaths.Add("ClientNetwork/Private");
        
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

        PublicIncludePaths.AddRange(
            new string[] {
                "Common"
            }
        );
    }
}
