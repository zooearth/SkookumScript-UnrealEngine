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
using System.Net;
using System.Collections.Generic;
using UnrealBuildTool;


public class AgogCore : ModuleRules
{
  public AgogCore(ReadOnlyTargetRules Target) : base(Target)
  {
    // Check if Sk source code is present (Pro-RT license) 
    var bFullSource = File.Exists(Path.Combine(ModuleDirectory, "..", "SkookumScript", "Private", "SkookumScript", "Sk.cpp"));
    // Allow packaging script to force a lib build by creating a temp file (Agog Labs internal)
    bFullSource = bFullSource && !File.Exists(Path.Combine(ModuleDirectory, "..", "SkookumScript", "force-lib-build.txt"));

    // If full source is present, build module from source, otherwise link with binary library
    Type = bFullSource ? ModuleType.CPlusPlus : ModuleType.External;

    // Enable fussy level of checking (Agog Labs internal)
    var bMadCheck = File.Exists(Path.Combine(ModuleDirectory, "enable-mad-check.txt"));
    if (bMadCheck)
    {
      Definitions.Add("A_MAD_CHECK");
    }

    // Add user define if exists (Agog Labs internal)
    var userDefineFilePath = Path.Combine(ModuleDirectory, "mad-define.txt");
    if (File.Exists(userDefineFilePath))
    {
      var userDefine = File.ReadAllText(userDefineFilePath).Trim();
      if (userDefine.Length > 0)
      {
        Definitions.Add(userDefine);
      }
    }

    var bPlatformAllowed = false;

    List<string> platPathSuffixes = new List<string>();

    string libNameExt = ".a";
    string libNamePrefix = "lib";
    string libNameSuffix = "";
    string platformName = "";
    bool useDebugCRT = BuildConfiguration.bDebugBuildsActuallyUseDebugCRT;

    switch (Target.Platform)
    {
      case UnrealTargetPlatform.Win32:
      case UnrealTargetPlatform.Win64:
        bPlatformAllowed = true;
        platformName = Target.Platform == UnrealTargetPlatform.Win64 ? "Win64" : "Win32";
        platPathSuffixes.Add(Path.Combine(platformName, WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2015 ? "VS2015" : "VS2013"));
        libNameExt = ".lib";
        libNamePrefix = "";
        break;
      case UnrealTargetPlatform.Mac:
        bPlatformAllowed = true;
        platformName = "Mac";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        Definitions.Add("A_PLAT_OSX");
        break;
      case UnrealTargetPlatform.IOS:
        bPlatformAllowed = true;
        platformName = "IOS";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        Definitions.Add("A_PLAT_iOS");
        break;
      case UnrealTargetPlatform.TVOS:
        bPlatformAllowed = true;
        platformName = "TVOS";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        Definitions.Add("A_PLAT_tvOS");
        break;
      case UnrealTargetPlatform.Android:
        bPlatformAllowed = true;
        platformName = "Android";
        platPathSuffixes.Add(Path.Combine(platformName, "ARM"));
        platPathSuffixes.Add(Path.Combine(platformName, "ARM64"));
        platPathSuffixes.Add(Path.Combine(platformName, "x86"));
        platPathSuffixes.Add(Path.Combine(platformName, "x64"));
        useDebugCRT = true;
        Definitions.Add("A_PLAT_ANDROID");
        break;
      case UnrealTargetPlatform.XboxOne:
        bPlatformAllowed = bFullSource;
        platformName = "XONE";
        Definitions.Add("A_PLAT_X_ONE");
        break;
      case UnrealTargetPlatform.Linux:
        bPlatformAllowed = true;
        platformName = "Linux";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        Definitions.Add("A_PLAT_LINUX64");
        //UEBuildConfiguration.bForceEnableExceptions = true;
        break;
    }

    // NOTE: All modules inside the SkookumScript plugin folder must use the exact same definitions!
    switch (Target.Configuration)
    {
      case UnrealTargetConfiguration.Debug:
      case UnrealTargetConfiguration.DebugGame:
        libNameSuffix = useDebugCRT ? "-Debug" : "-DebugCRTOpt";
        Definitions.Add("A_EXTRA_CHECK=1");
        Definitions.Add("A_UNOPTIMIZED=1");
        break;

      case UnrealTargetConfiguration.Development:
      case UnrealTargetConfiguration.Test:
        libNameSuffix = "-Development";
        Definitions.Add("A_EXTRA_CHECK=1");
        break;

      case UnrealTargetConfiguration.Shipping:
        libNameSuffix = "-Shipping";
        Definitions.Add("A_SYMBOL_STR_DB=1");
        Definitions.Add("A_NO_SYMBOL_REF_LINK=1");
        break;
    }

    // Determine if monolithic build
    var bIsMonolithic = (Target.LinkType == TargetLinkType.Monolithic);

    if (!bIsMonolithic)
    {
      Definitions.Add("A_IS_DLL");
    }

    // Public include paths
    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

    if (bFullSource)
    {
      // We're building SkookumScript from source - not much else needed
      PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
      // Build system wants us to be dependent on some module with precompiled headers, so be it
      PrivateDependencyModuleNames.Add("Core");
    }
    else if (bPlatformAllowed)
    {
      var moduleName = "AgogCore";
      // Link with monolithic library on all platforms except UE4Editor Win64 which requires a specific DLL import library
      var libFileNameStem = libNamePrefix + moduleName + ((!bIsMonolithic && Target.Platform == UnrealTargetPlatform.Win64) ? "-" + platformName : "") + libNameSuffix;
      var libFileName = libFileNameStem + libNameExt;
      var libDirPathBase = Path.Combine(ModuleDirectory, "Lib");
      // Add library paths to linker parameters
      foreach (var platPathSuffix in platPathSuffixes)
      {
        var libDirPath = Path.Combine(libDirPathBase, platPathSuffix);
        var libFilePath = Path.Combine(libDirPath, libFileName);

        PublicLibraryPaths.Add(libDirPath);

        // For non-Android, add full path
        if (Target.Platform != UnrealTargetPlatform.Android)
        {
          PublicAdditionalLibraries.Add(libFilePath);
        }
      }

      // For Android, just add core of library name, e.g. "SkookumScript-Development"
      if (Target.Platform == UnrealTargetPlatform.Android)
      {
        PublicAdditionalLibraries.Add(moduleName + libNameSuffix);
      }
    }
  }
}
