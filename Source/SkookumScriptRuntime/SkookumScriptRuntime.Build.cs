//=======================================================================================
// Copyright (c) 2001-2017 Agog Labs Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=======================================================================================

//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
//=======================================================================================


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

      // Tell build system we're not using PCHs
      PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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

      if (UEBuildConfiguration.bBuildEditor)
        {
        PrivateDependencyModuleNames.Add("UnrealEd");
        PrivateDependencyModuleNames.Add("KismetCompiler");
        PrivateDependencyModuleNames.Add("SourceControl");
      }

      // Load SkookumScript.ini and add any ScriptSupportedModules specified to the list of PrivateDependencyModuleNames
      PublicDependencyModuleNames.AddRange(GetSkookumScriptModuleNames(Path.Combine(ModuleDirectory, "../.."), false));

      // Add any modules that your module loads dynamically here ...
      //DynamicallyLoadedModuleNames.AddRange(new string[] {});
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
}
