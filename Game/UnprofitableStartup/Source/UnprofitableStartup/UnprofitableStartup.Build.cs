// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnprofitableStartup : ModuleRules
{
	public UnprofitableStartup(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        // Add UMG so we can use UMG UI
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Support for JSON files
        PublicDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });

        // This does not seem to be needed when just using UMG UI, but per tutorial, uncomment as well
        // https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/UMG/
        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
