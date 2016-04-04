// Copyright (c) 2015 Agog Labs, Inc. All Rights Reserved.

using System.IO;
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
            //"CoreUObject",
            "InputCore",
            "SlateCore",
            "Slate",
            "MessagingRpc",
            "PortalRpc",
            "PortalServices",
            "EngineMessages",
            "EngineSettings",
            //"Engine",
            "MediaAssets",
            "MoviePlayer",
            "Serialization",
            "HeadMountedDisplay",
            "AutomationMessages",
            "MovieScene",
            "LevelSequence",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "JsonUtilities",
            "MovieSceneTracks",
            "MovieSceneCapture",
            "SessionMessages",
            "UMG",
            "MaterialShaderQualitySettings",
            "Foliage",
            "Landscape",
            "VectorVM",
            "Niagara",
            "AnimGraphRuntime",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "WebBrowser",
            "GameplayAbilities",

            "Sockets",
            "HTTP",
            "Networking",
            "NetworkReplayStreaming",
            "Projects",
            "AgogCore",
            "SkookumScript",
          }
        );

      // Add any modules that your module loads dynamically here ...
      //DynamicallyLoadedModuleNames.AddRange(new string[] {});
    }
  }
}