using UnrealBuildTool;
using System.Collections.Generic;
public class StoneyWorldEditorTarget : TargetRules
{
    public StoneyWorldEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.Add("StoneyWorld");
    }
}
