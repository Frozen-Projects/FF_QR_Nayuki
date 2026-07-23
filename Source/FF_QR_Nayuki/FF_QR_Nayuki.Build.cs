using System;
using System.IO;
using UnrealBuildTool;

public class FF_QR_Nayuki : ModuleRules
{
	public void Load_Nayuki()
    {
        string NayukiQRPath = Path.Combine(ModuleDirectory, "ThirdParty", "nayuki_qr");
        PrivateIncludePaths.Add(NayukiQRPath);
    }

    public FF_QR_Nayuki(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        bUseUnity = false;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            bUseRTTI = true;
        }

        Load_Nayuki();

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
            });
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
            });
	}
}