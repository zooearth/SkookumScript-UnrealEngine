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

#include "SkookumStyles.h"
#include "Interfaces/IPluginManager.h"
#include "SlateGameResources.h"
#include "SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH( RelativePath, ... )  FSlateImageBrush(ms_singleton_p->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH( RelativePath, ... )    FSlateBoxBrush(ms_singleton_p->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush(ms_singleton_p->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT( RelativePath, ... )     FSlateFontInfo(ms_singleton_p->RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT( RelativePath, ... )     FSlateFontInfo(ms_singleton_p->RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FSkookumStyles::ms_singleton_p = NULL;

void FSkookumStyles::Initialize()
  {
  if (!ms_singleton_p.IsValid())
    {
    const FVector2D Icon20x20(20.0f, 20.0f);
    const FVector2D Icon40x40(40.0f, 40.0f);

    FString plugin_root_path(IPluginManager::Get().FindPlugin(TEXT("SkookumScript"))->GetBaseDir());
    FString content_path = plugin_root_path / TEXT("Content/GUI");
    ms_singleton_p = FSlateGameResources::New(FSkookumStyles::GetStyleSetName(), content_path, content_path);

    // toolbar icons
    ms_singleton_p->Set("SkookumScriptEditor.ShowIDE_Connected", new IMAGE_BRUSH("icon_ide_connected_40x", Icon40x40));
    ms_singleton_p->Set("SkookumScriptEditor.ShowIDE_Disconnected", new IMAGE_BRUSH("icon_ide_disconnected_40x", Icon40x40));
    ms_singleton_p->Set("SkookumScriptEditor.ShowIDE_Connected.Small", new IMAGE_BRUSH("icon_ide_connected_40x", Icon20x20));
    ms_singleton_p->Set("SkookumScriptEditor.ShowIDE_Disconnected.Small", new IMAGE_BRUSH("icon_ide_disconnected_40x", Icon20x20));

    FSlateStyleRegistry::RegisterSlateStyle(*ms_singleton_p);
    }
  }

void FSkookumStyles::Shutdown()
  {
  FSlateStyleRegistry::UnRegisterSlateStyle(*ms_singleton_p);
  ensure(ms_singleton_p.IsUnique());
  ms_singleton_p.Reset();
  }

FName FSkookumStyles::GetStyleSetName()
  {
  static FName StyleSetName(TEXT("SkookumStyles"));
  return StyleSetName;
  }

const ISlateStyle& FSkookumStyles::Get()
  {
  return *ms_singleton_p;
  }
