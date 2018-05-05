// Copyright 2018 by NiHongjian. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ToAmuseEditorTarget : TargetRules
{
	public ToAmuseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "ToAmuse" } );
	}
}
