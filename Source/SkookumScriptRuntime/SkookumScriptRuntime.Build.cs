// Copyright (c) 2014-2016 Agog Labs, Inc. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
  public class SkookumScriptRuntime : ModuleRules
  {
    public SkookumScriptRuntime(TargetInfo Target)
    {

      // SkUEBindings.cpp takes a long time to compile due to auto-generated engine bindings
      // Set to true when actively working on this plugin, false otherwise
      bFasterWithoutUnity = false;

      // Add public include paths required here ...
      PublicIncludePaths.Add("SkookumScriptRuntime/Public/Bindings");
      //PublicIncludePaths.AddRange(
      //  new string[] {          
      //    //"Programs/UnrealHeaderTool/Public",
      //    }
      //  );

      PrivateIncludePaths.Add("SkookumScriptRuntime/Private");

      // Add public dependencies that you statically link with here ...
      PublicDependencyModuleNames.AddRange(
        new string[]
          {
            "Core",
            "CoreUObject",
            "Engine",
          }
        );

      if (UEBuildConfiguration.bBuildEditor == true)
      {
        PublicDependencyModuleNames.Add("UnrealEd");
      }

      // ... add private dependencies that you statically link with here ...
      PrivateDependencyModuleNames.AddRange(
        new string[]
          {
            "Sockets",
            "HTTP",
            "Networking",
            "NetworkReplayStreaming",
            "Projects",
          }
        );

      // Load SkookumScript.ini and add any ScriptSupportedModules specified to the list of PrivateDependencyModuleNames
      PrivateDependencyModuleNames.AddRange(GetSkookumScriptModuleNames(Path.Combine(ModuleDirectory, "../.."), false));

      // Add any modules that your module loads dynamically here ...
      //DynamicallyLoadedModuleNames.AddRange(new string[] {});
    }

    // Load SkookumScript.ini and return any ScriptSupportedModules specified
    public static List<string> GetSkookumScriptModuleNames(string PluginOrProjectRootDirectory, bool AddSkookumScriptRuntime = true)
    {
      List<string> moduleList = new List<string>();

      // Load SkookumScript.ini and get ScriptSupportedModules
      string iniFilePath = Path.Combine(PluginOrProjectRootDirectory, "Config/SkookumScript.ini");
      if (File.Exists(iniFilePath))
      {
        ConfigCacheIni ini = new ConfigCacheIni(new FileReference(iniFilePath));
        ini.GetArray("CommonSettings", "ScriptSupportedModules", out moduleList);
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
}
