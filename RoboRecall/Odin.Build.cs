// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Collections.Generic;

public class Odin : ModuleRules
{
	public Odin(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",
            "GameplayTasks",
            "PhysX", 
            "APEX",
            "HeadMountedDisplay",
            "OnlineSubsystemOculus"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "InputDevice",
            "OculusInput",
            "CableComponent",
            "OculusLibrary",
            "UMG",
            "RHI",
			"Projects",
            "RenderCore",
            "OnlineSubsystem",
            "LibOVRPlatform",
			"Analytics",
			"AnalyticsET",
			"EpicGameAnalytics",
			"EngineSettings",
			"SlateCore",
        });

		// Load SkookumScript.ini and add any ScriptSupportedModules specified to the list of PrivateDependencyModuleNames
		PrivateDependencyModuleNames.AddRange(GetSkookumScriptModuleNames(Path.Combine(ModuleDirectory, "../..")));

        PublicIncludePaths.Add(System.IO.Path.Combine(ModuleDirectory, "Game"));

/*        DynamicallyLoadedModuleNames.Add("OnlineSubsystemOculus");*/

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }
    }

	// Load SkookumScript.ini and return any ScriptSupportedModules specified
	public static List<string> GetSkookumScriptModuleNames(string PluginOrProjectRootDirectory, bool AddSkookumScriptRuntime = true)
	{
		List<string> moduleList = null;

		// Load SkookumScript.ini and get ScriptSupportedModules
		string iniFilePath = Path.Combine(PluginOrProjectRootDirectory, "Config/SkookumScript.ini");
		if (File.Exists(iniFilePath))
		{
			ConfigFile iniFile = new ConfigFile(new FileReference(iniFilePath), ConfigLineAction.Add);
			var skookumConfig = new ConfigHierarchy(new ConfigFile[] { iniFile });
			skookumConfig.GetArray("CommonSettings", "ScriptSupportedModules", out moduleList);
		}

		if (moduleList == null)
		{
			moduleList = new List<string>();
		}

		// Add additional modules needed for SkookumScript to function
		moduleList.Add("AgogCore");
		moduleList.Add("SkookumScript");
		if (AddSkookumScriptRuntime)
		{
			moduleList.Add("SkookumScriptRuntime");
		}

		return moduleList;
	}
}
