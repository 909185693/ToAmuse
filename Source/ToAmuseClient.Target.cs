// Copyright 2018 by January. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ToAmuseClientTarget : TargetRules
{
	public ToAmuseClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange( new string[] { "ToAmuse" } );
	}
}
