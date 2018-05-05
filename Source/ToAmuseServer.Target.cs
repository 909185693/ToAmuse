// Copyright 2018 by NiHongjian. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ToAmuseServerTarget : TargetRules
{
	public ToAmuseServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

        // No Use Slate
        bUsesSlate = false;

        // Turn on shipping logging
        bUseLoggingInShipping = true;

        ExtraModuleNames.AddRange( new string[] { "ToAmuse" } );
	}
}
