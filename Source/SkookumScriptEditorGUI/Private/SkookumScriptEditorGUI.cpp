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

#include "SkookumScriptEditorGUIPrivatePCH.h"

#include "ISkookumScriptEditorGUI.h"
#include "ISkookumScriptRuntime.h"

#include "SkookumScriptEditorCommands.h"
#include "SkookumStyles.h"
#include "LevelEditor.h"
#include "GraphEditor.h"
#include "BlueprintEditor.h"
#include "BlueprintEditorModule.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Event.h"
#include "MultiBoxExtender.h"
#include "SlateStyleRegistry.h"
#include "Misc/AssertionMacros.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"

DEFINE_LOG_CATEGORY_STATIC(LogSkookumScriptEditorGUI, Log, All);

//---------------------------------------------------------------------------------------

class FSkookumScriptEditorGUI : public ISkookumScriptEditorGUI
{
public:

protected:

  //---------------------------------------------------------------------------------------
  // IModuleInterface implementation

  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

  //---------------------------------------------------------------------------------------
  // Local implementation

  ISkookumScriptRuntime * get_runtime() const { return static_cast<ISkookumScriptRuntime *>(m_runtime_p.Get()); }

  void                    add_skookum_button_to_level_tool_bar(FToolBarBuilder &);
  void                    add_skookum_button_to_blueprint_tool_bar(FToolBarBuilder &);
  void                    on_skookum_button_clicked();

  // Helper class
  class SharedHelper
    {
    public:      
      SharedHelper(ISkookumScriptRuntime * runtime_p) : m_runtime_p(runtime_p) {}

      FSlateIcon  get_sk_icon() const;
      FText       get_lvl_tooltip() const;
      FText       get_bp_tooltip() const;
      
      void        on_application_focus_changed(const bool is_active);

    protected:
      ISkookumScriptRuntime * m_runtime_p;

    };

  // Data members

  TSharedPtr<IModuleInterface>      m_runtime_p;  // TSharedPtr holds on to the module so it can't go away while we need it

  TSharedPtr<FUICommandList>        m_button_commands;

  TSharedPtr<FExtensibilityManager> m_level_extension_manager;
  TSharedPtr<const FExtensionBase>  m_level_tool_bar_extension;
  TSharedPtr<FExtender>             m_level_tool_bar_extender;

  TSharedPtr<FExtensibilityManager> m_blueprint_extension_manager;
  TSharedPtr<const FExtensionBase>  m_blueprint_tool_bar_extension;
  TSharedPtr<FExtender>             m_blueprint_tool_bar_extender;

  FDelegateHandle                   m_on_application_focus_changed_handle;

  TSharedPtr<SharedHelper>          m_helper_p;
  };

IMPLEMENT_MODULE(FSkookumScriptEditorGUI, SkookumScriptEditorGUI)

//=======================================================================================
// IModuleInterface implementation
//=======================================================================================

//---------------------------------------------------------------------------------------

void FSkookumScriptEditorGUI::StartupModule()
  {
  // Get pointer to runtime module
  m_runtime_p = FModuleManager::Get().GetModule("SkookumScriptRuntime");

  // Don't do anything if SkookumScript is not active
  if (get_runtime()->is_skookum_disabled())
    {
    return;
    }

  if (!IsRunningCommandlet())
    {
    // Register commands and styles
    FSkookumScriptEditorCommands::Register();
    FSlateStyleRegistry::UnRegisterSlateStyle(FSkookumStyles::GetStyleSetName()); // Hot reload hack
    FSkookumStyles::Initialize();

    // Button commands 
    m_button_commands = MakeShareable(new FUICommandList);
    m_button_commands->MapAction(
      FSkookumScriptEditorCommands::Get().m_skookum_button,
      FExecuteAction::CreateRaw(this, &FSkookumScriptEditorGUI::on_skookum_button_clicked),
      FCanExecuteAction());

    // Add to level tool bar
    m_level_tool_bar_extender = MakeShareable(new FExtender);
    m_level_tool_bar_extension = m_level_tool_bar_extender->AddToolBarExtension("Compile", EExtensionHook::After, m_button_commands, FToolBarExtensionDelegate::CreateRaw(this, &FSkookumScriptEditorGUI::add_skookum_button_to_level_tool_bar));
    FLevelEditorModule & level_editor_module = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    m_level_extension_manager = level_editor_module.GetToolBarExtensibilityManager();
    m_level_extension_manager->AddExtender(m_level_tool_bar_extender);

    // Add to blueprint tool bar
    m_blueprint_tool_bar_extender = MakeShareable(new FExtender);
    m_blueprint_tool_bar_extension = m_blueprint_tool_bar_extender->AddToolBarExtension("Asset", EExtensionHook::After, m_button_commands, FToolBarExtensionDelegate::CreateRaw(this, &FSkookumScriptEditorGUI::add_skookum_button_to_blueprint_tool_bar));
    FBlueprintEditorModule & blueprint_editor_module = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
    m_blueprint_extension_manager = blueprint_editor_module.GetMenuExtensibilityManager();
    m_blueprint_extension_manager->AddExtender(m_blueprint_tool_bar_extender);

    // Create helper
    m_helper_p = MakeShareable(new SharedHelper(get_runtime()));

    // Hook into application focus change event
    m_on_application_focus_changed_handle = FSlateApplication::Get().OnApplicationActivationStateChanged().AddSP(m_helper_p.ToSharedRef(), &SharedHelper::on_application_focus_changed);
    }
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditorGUI::ShutdownModule()
  {
  // Don't do anything if SkookumScript is not active
  if (get_runtime()->is_skookum_disabled())
    {
    return;
    }
  
  get_runtime()->set_editor_interface(nullptr);
  m_runtime_p.Reset();

  if (!IsRunningCommandlet())
    {
    // FSlateApplication has already been deleted at this point
    // FSlateApplication::Get().OnApplicationActivationStateChanged().Remove(m_on_application_focus_changed_handle);

    if (m_level_extension_manager.IsValid())
      {
      FSkookumScriptEditorCommands::Unregister();
      m_level_tool_bar_extender->RemoveExtension(m_level_tool_bar_extension.ToSharedRef());
      m_level_extension_manager->RemoveExtender(m_level_tool_bar_extender);
      }
    else
      {
      m_level_extension_manager.Reset();
      }

    if (m_blueprint_extension_manager.IsValid())
      {
      FSkookumScriptEditorCommands::Unregister();
      m_blueprint_tool_bar_extender->RemoveExtension(m_blueprint_tool_bar_extension.ToSharedRef());
      m_blueprint_extension_manager->RemoveExtender(m_blueprint_tool_bar_extender);
      }
    else
      {
      m_blueprint_extension_manager.Reset();
      }

    FSkookumStyles::Shutdown();
    }
  }

//=======================================================================================
// FSkookumScriptEditorGUI implementation
//=======================================================================================

//---------------------------------------------------------------------------------------

void FSkookumScriptEditorGUI::add_skookum_button_to_level_tool_bar(FToolBarBuilder & builder)
  {
  #define LOCTEXT_NAMESPACE "LevelEditorToolBar"

    builder.AddToolBarButton(
      FSkookumScriptEditorCommands::Get().m_skookum_button, 
      NAME_None, 
      LOCTEXT("SkookumButton_Override", "SkookumIDE"), 
      TAttribute<FText>(m_helper_p.ToSharedRef(), &SharedHelper::get_lvl_tooltip),
      TAttribute<FSlateIcon>(m_helper_p.ToSharedRef(), &SharedHelper::get_sk_icon),
      NAME_None);

  #undef LOCTEXT_NAMESPACE
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditorGUI::add_skookum_button_to_blueprint_tool_bar(FToolBarBuilder & builder)
  {
  #define LOCTEXT_NAMESPACE "BlueprintEditorToolBar"

  builder.AddToolBarButton(
    FSkookumScriptEditorCommands::Get().m_skookum_button,
    NAME_None,
    LOCTEXT("SkookumButton_Override", "Show in IDE"),
    TAttribute<FText>(m_helper_p.ToSharedRef(), &SharedHelper::get_bp_tooltip),
    TAttribute<FSlateIcon>(m_helper_p.ToSharedRef(), &SharedHelper::get_sk_icon),
    NAME_None);

  #undef LOCTEXT_NAMESPACE
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditorGUI::on_skookum_button_clicked()
  {
  FString focus_class_name;
  FString focus_member_name;

  // There must be a better way of finding the active editor...
  TArray<UObject*> obj_array = FAssetEditorManager::Get().GetAllEditedAssets();
  double latest_activation_time = 0.0;
  IAssetEditorInstance * active_editor_p = nullptr;
  if (obj_array.Num() > 0)
    {
    for (auto obj_iter = obj_array.CreateConstIterator(); obj_iter; ++obj_iter)
      {
      // Don't allow user to perform certain actions on objects that aren't actually assets (e.g. Level Script blueprint objects)
      UBlueprint * blueprint_p = Cast<UBlueprint>(*obj_iter);
      if (blueprint_p)
        {
        IAssetEditorInstance * editor_p = FAssetEditorManager::Get().FindEditorForAsset(blueprint_p, false);
        if (editor_p->GetLastActivationTime() > latest_activation_time)
          {
          latest_activation_time = editor_p->GetLastActivationTime();
          active_editor_p = editor_p;
          }
        }
      }
    }

  if (active_editor_p)
    {
    // We found the most recently used Blueprint editor
    FBlueprintEditor * blueprint_editor_p = static_cast<FBlueprintEditor *>(active_editor_p);
    // Get name of associated Blueprint
    focus_class_name = blueprint_editor_p->GetBlueprintObj()->GetName();
    // See if any SkookumScript node is selected, if so, tell the IDE
    const FGraphPanelSelectionSet node_array = blueprint_editor_p->GetSelectedNodes();
    for (auto obj_iter = node_array.CreateConstIterator(); obj_iter; ++obj_iter)
      {
      UK2Node_CallFunction * call_node_p = Cast<UK2Node_CallFunction>(*obj_iter);
      if (call_node_p)
        {
        focus_member_name = call_node_p->FunctionReference.GetMemberName().ToString();
        break; // For now, just return the first one
        }
      UK2Node_Event * event_node_p = Cast<UK2Node_Event>(*obj_iter);
      if (event_node_p)
        {
        focus_member_name = event_node_p->EventReference.GetMemberName().ToString();
        break; // For now, just return the first one
        }
      }
    }

  // Bring up IDE and navigate to selected class/method/coroutine
  get_runtime()->show_ide(focus_class_name, focus_member_name, false, false);

  // Request recompilation if there have previously been errors
  get_runtime()->freshen_compiled_binaries_if_have_errors();
  }

//---------------------------------------------------------------------------------------

FSlateIcon FSkookumScriptEditorGUI::SharedHelper::get_sk_icon() const
  {
  return m_runtime_p->is_connected_to_ide()
    ? FSlateIcon(FSkookumStyles::GetStyleSetName(), "SkookumScriptEditor.ShowIDE_Connected", "SkookumScriptEditor.ShowIDE_Connected.Small")
    : FSlateIcon(FSkookumStyles::GetStyleSetName(), "SkookumScriptEditor.ShowIDE_Disconnected", "SkookumScriptEditor.ShowIDE_Disconnected.Small");
  }

//---------------------------------------------------------------------------------------

FText FSkookumScriptEditorGUI::SharedHelper::get_lvl_tooltip() const
  {
  #define LOCTEXT_NAMESPACE "LevelEditorToolBar"

  return m_runtime_p->is_connected_to_ide()
    ? LOCTEXT("SkookumButton_ToolTipOverride", "Summons the SkookumIDE and navigates to the related class")
    : LOCTEXT("SkookumButton_ToolTipOverride", "Press to connect to the SkookumIDE");

  #undef LOCTEXT_NAMESPACE
  }

//---------------------------------------------------------------------------------------

FText FSkookumScriptEditorGUI::SharedHelper::get_bp_tooltip() const
  {
  #define LOCTEXT_NAMESPACE "BlueprintEditorToolBar"

  return m_runtime_p->is_connected_to_ide()
    ? LOCTEXT("SkookumButton_ToolTipOverride", "Summons the SkookumIDE and navigates to the related class or method/coroutine")
    : LOCTEXT("SkookumButton_ToolTipOverride", "Press to connect to the SkookumIDE");

  #undef LOCTEXT_NAMESPACE
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditorGUI::SharedHelper::on_application_focus_changed(const bool is_active)
  {
  m_runtime_p->on_application_focus_changed(is_active);
  }
