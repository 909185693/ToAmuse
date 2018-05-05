// Copyright 2018 by NiHongjian. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ToAmuseTarget : TargetRules
{
	public ToAmuseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ToAmuse" } );
	}
}
