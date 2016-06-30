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
            // All Engine/Runtime modules
            "AIModule",
            "AnalyticsVisualEditing",
            "AndroidRuntimeSettings",
            "AnimGraphRuntime",
            "AutomationMessages",
            "CinematicCamera",
            //"CoreUObject",
            //"Engine",
            "EngineMessages",
            "EngineSettings",
            "Foliage",
            "GameLiveStreaming",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            //"GeometryCache",
            "HeadMountedDisplay",
            //"IOSRuntimeSettings",
            "LaunchDaemonMessages",
            "InputCore",
            "JsonUtilities",
            "Landscape",
            "LevelSequence",
            "MaterialShaderQualitySettings",
            "MediaAssets",
            "MessagingRpc",
            "MoviePlayer",
            "MovieScene",
            "MovieSceneCapture",
            "MovieSceneTracks",
            "Niagara",
            "OnlineSubsystem",
            "OnlineSubsystemSteam",
            "OnlineSubsystemUtils",
            "PacketHandler",
            "PortalMessages",
            "PortalRpc",
            "PortalServices",
            "Serialization",
            "SessionMessages",
            "Slate",
            "SlateCore",
            "UMG",
            "VectorVM",
            "WebBrowser",

            // Extra modules we need
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