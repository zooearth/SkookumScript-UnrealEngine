//=======================================================================================
// SkookumScript Unreal Engine Editor Plugin
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Author: Markus Breyer
//=======================================================================================

#include "SkookumScriptEditorPrivatePCH.h"
#include <ISkookumScriptRuntime.h>
#include <AssetRegistryModule.h>
#include <BlueprintActionDatabase.h>
#include <BlueprintEditorUtils.h>
#include <K2Node_CallFunction.h>
#include <K2Node_Event.h>

#include "GraphEditor.h"

#include "../../SkookumScriptGenerator/Private/SkookumScriptGeneratorBase.inl"

DEFINE_LOG_CATEGORY(LogSkookumScriptEditor);

//---------------------------------------------------------------------------------------

class FSkookumScriptEditor : public ISkookumScriptEditor, public ISkookumScriptRuntimeEditorInterface
{
public:

protected:

  //---------------------------------------------------------------------------------------
  // IModuleInterface implementation

  virtual void  StartupModule() override;
  virtual void  ShutdownModule() override;

  //---------------------------------------------------------------------------------------
  // ISkookumScriptRuntimeEditorInterface implementation

  virtual void  recompile_blueprints_with_errors() const override;
  virtual void  on_class_updated(UClass * ue_class_p) override;
  virtual bool  check_out_file(const FString & file_path) const override;

  //---------------------------------------------------------------------------------------
  // Local implementation

  ISkookumScriptRuntime * get_runtime() const { return static_cast<ISkookumScriptRuntime *>(m_runtime_p.Get()); }

  void                    on_asset_loaded(UObject * obj_p);
  void                    on_object_modified(UObject * obj_p);
  void                    on_new_asset_created(UFactory * factory_p);
  void                    on_assets_deleted(const TArray<UClass*> & deleted_asset_classes);
  void                    on_asset_post_import(UFactory * factory_p, UObject * obj_p);
  void                    on_asset_added(const FAssetData & asset_data);
  void                    on_asset_renamed(const FAssetData & asset_data, const FString & old_object_path);
  void                    on_in_memory_asset_created(UObject * obj_p);
  void                    on_in_memory_asset_deleted(UObject * obj_p);
  void                    on_map_opened(const FString & file_name, bool as_template);
  void                    on_blueprint_compiled(UBlueprint * blueprint_p);

  void                    on_new_asset(UObject * obj_p);

  // Data members

  TSharedPtr<IModuleInterface>  m_runtime_p;  // TSharedPtr holds on to the module so it can't go away while we need it

  FDelegateHandle               m_on_asset_loaded_handle;
  FDelegateHandle               m_on_object_modified_handle;
  FDelegateHandle               m_on_map_opened_handle;
  FDelegateHandle               m_on_new_asset_created_handle;
  FDelegateHandle               m_on_assets_deleted_handle;
  FDelegateHandle               m_on_asset_post_import_handle;
  FDelegateHandle               m_on_asset_added_handle;
  FDelegateHandle               m_on_asset_renamed_handle;
  FDelegateHandle               m_on_in_memory_asset_created_handle;
  FDelegateHandle               m_on_in_memory_asset_deleted_handle;

  };

IMPLEMENT_MODULE(FSkookumScriptEditor, SkookumScriptEditor)

//=======================================================================================
// IModuleInterface implementation
//=======================================================================================

//---------------------------------------------------------------------------------------

void FSkookumScriptEditor::StartupModule()
  {
  // Get pointer to runtime module
  m_runtime_p = FModuleManager::Get().GetModule("SkookumScriptRuntime");

  // Don't do anything if SkookumScript is not active
  if (get_runtime()->is_skookum_disabled())
    {
    return;
    }

  // Tell runtime that editor is present (needed even in commandlet mode as we might have to demand-load blueprints)
  get_runtime()->set_editor_interface(this);

  if (!IsRunningCommandlet())
    {
    // Hook up delegates
    m_on_asset_loaded_handle = FCoreUObjectDelegates::OnAssetLoaded.AddRaw(this, &FSkookumScriptEditor::on_asset_loaded);
    m_on_object_modified_handle = FCoreUObjectDelegates::OnObjectModified.AddRaw(this, &FSkookumScriptEditor::on_object_modified);
    m_on_map_opened_handle = FEditorDelegates::OnMapOpened.AddRaw(this, &FSkookumScriptEditor::on_map_opened);
    m_on_new_asset_created_handle = FEditorDelegates::OnNewAssetCreated.AddRaw(this, &FSkookumScriptEditor::on_new_asset_created);
    m_on_assets_deleted_handle = FEditorDelegates::OnAssetsDeleted.AddRaw(this, &FSkookumScriptEditor::on_assets_deleted);
    m_on_asset_post_import_handle = FEditorDelegates::OnAssetPostImport.AddRaw(this, &FSkookumScriptEditor::on_asset_post_import);

    FAssetRegistryModule & asset_registry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
    m_on_asset_added_handle = asset_registry.Get().OnAssetAdded().AddRaw(this, &FSkookumScriptEditor::on_asset_added);
    m_on_asset_renamed_handle = asset_registry.Get().OnAssetRenamed().AddRaw(this, &FSkookumScriptEditor::on_asset_renamed);
    m_on_in_memory_asset_created_handle = asset_registry.Get().OnInMemoryAssetCreated().AddRaw(this, &FSkookumScriptEditor::on_in_memory_asset_created);
    m_on_in_memory_asset_deleted_handle = asset_registry.Get().OnInMemoryAssetDeleted().AddRaw(this, &FSkookumScriptEditor::on_in_memory_asset_deleted);
    }

  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditor::ShutdownModule()
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
    // Remove delegates
    FCoreUObjectDelegates::OnAssetLoaded.Remove(m_on_asset_loaded_handle);
    FCoreUObjectDelegates::OnObjectModified.Remove(m_on_object_modified_handle);
    FEditorDelegates::OnMapOpened.Remove(m_on_map_opened_handle);
    FEditorDelegates::OnNewAssetCreated.Remove(m_on_new_asset_created_handle);
    FEditorDelegates::OnAssetsDeleted.Remove(m_on_assets_deleted_handle);
    FEditorDelegates::OnAssetPostImport.Remove(m_on_asset_post_import_handle);

    FAssetRegistryModule * asset_registry_p = FModuleManager::GetModulePtr<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
    if (asset_registry_p)
      {
      asset_registry_p->Get().OnAssetAdded().Remove(m_on_asset_added_handle);
      asset_registry_p->Get().OnAssetRenamed().Remove(m_on_asset_renamed_handle);
      asset_registry_p->Get().OnInMemoryAssetCreated().Remove(m_on_in_memory_asset_created_handle);
      asset_registry_p->Get().OnInMemoryAssetDeleted().Remove(m_on_in_memory_asset_deleted_handle);
      }
    }
  }

//=======================================================================================
// ISkookumScriptRuntimeEditorInterface implementation
//=======================================================================================

//---------------------------------------------------------------------------------------
// Find blueprints that have compile errors and try recompiling them
// (as errors might be due to SkookumScript not having been initialized at previous compile time
void FSkookumScriptEditor::recompile_blueprints_with_errors() const
  {
  TArray<UObject*> blueprint_array;
  GetObjectsOfClass(UBlueprint::StaticClass(), blueprint_array, false, RF_ClassDefaultObject);
  for (UObject * obj_p : blueprint_array)
    {
    UBlueprint * blueprint_p = static_cast<UBlueprint *>(obj_p);
    if (blueprint_p->Status == BS_Error)
      {
      FKismetEditorUtilities::CompileBlueprint(blueprint_p);
      }
    }
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditor::on_class_updated(UClass * ue_class_p)
  {
  // Nothing to do if no engine
  if (!GEngine) return;

  // 1) Refresh actions (in Blueprint editor drop down menu)
  FBlueprintActionDatabase::Get().RefreshClassActions(ue_class_p);

  // Storage for gathered objects
  TArray<UObject*> obj_array;

  // 2) Refresh node display of all SkookumScript function call nodes
  obj_array.Reset();
  GetObjectsOfClass(UK2Node_CallFunction::StaticClass(), obj_array, true, RF_ClassDefaultObject);
  for (auto obj_p : obj_array)
    {
    UK2Node_CallFunction * function_node_p = Cast<UK2Node_CallFunction>(obj_p);
    UFunction * target_function_p = function_node_p->GetTargetFunction();
    // Also refresh all nodes with no target function as it is probably a Sk function that was deleted
    //if (!target_function_p || get_runtime()->is_skookum_blueprint_function(target_function_p))
    if (target_function_p && get_runtime()->is_skookum_blueprint_function(target_function_p))
      {
      const UEdGraphSchema * schema_p = function_node_p->GetGraph()->GetSchema();
      schema_p->ReconstructNode(*function_node_p, true);
      }
    }

  // 3) Refresh node display of all SkookumScript event nodes
  obj_array.Reset();
  GetObjectsOfClass(UK2Node_Event::StaticClass(), obj_array, true, RF_ClassDefaultObject);
  for (auto obj_p : obj_array)
    {
    UK2Node_Event * event_node_p = Cast<UK2Node_Event>(obj_p);
    UFunction * event_function_p = event_node_p->FindEventSignatureFunction();
    if (event_function_p && get_runtime()->is_skookum_blueprint_event(event_function_p))
      {
      const UEdGraphSchema * schema_p = event_node_p->GetGraph()->GetSchema();
      schema_p->ReconstructNode(*event_node_p, true);
      }
    }

  // 4) Try recompiling any Blueprints that previously had errors
  recompile_blueprints_with_errors();
  }

//---------------------------------------------------------------------------------------

bool FSkookumScriptEditor::check_out_file(const FString & file_path) const
  {
  if (!ISourceControlModule::Get().IsEnabled())
    {
    return false;
    }

  return SourceControlHelpers::CheckOutFile(file_path);
  }

//=======================================================================================
// FSkookumScriptEditor implementation
//=======================================================================================

//---------------------------------------------------------------------------------------

void FSkookumScriptEditor::on_asset_loaded(UObject * obj_p)
  {
  on_new_asset(obj_p);
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_object_modified(UObject * obj_p)
  {
  // Is this a blueprint?
  UBlueprint * blueprint_p = Cast<UBlueprint>(obj_p);
  if (blueprint_p)
    {
    get_runtime()->generate_class_script_files(blueprint_p->GeneratedClass, true);
    }
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_new_asset_created(UFactory * factory_p)
  {
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_assets_deleted(const TArray<UClass*> & deleted_asset_classes)
  {
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_asset_post_import(UFactory * factory_p, UObject * obj_p)
  {
  UBlueprint * blueprint_p = Cast<UBlueprint>(obj_p);
  if (blueprint_p)
    {
    get_runtime()->generate_class_script_files(blueprint_p->GeneratedClass, true);
    get_runtime()->generate_used_class_script_files();
    }
  }

//---------------------------------------------------------------------------------------
// An asset is being added to the asset registry - during load, or otherwise
void FSkookumScriptEditor::on_asset_added(const FAssetData & asset_data)
  {
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_asset_renamed(const FAssetData & asset_data, const FString & old_object_path)
  {
  static FName s_blueprint_class_name(TEXT("Blueprint"));

  if (asset_data.AssetClass == s_blueprint_class_name)
    {
    UBlueprint * blueprint_p = FindObjectChecked<UBlueprint>(ANY_PACKAGE, *asset_data.AssetName.ToString());
    if (blueprint_p)
      {
      get_runtime()->rename_class_script_files(blueprint_p->GeneratedClass, FPaths::GetBaseFilename(old_object_path));
      }
    }
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_in_memory_asset_created(UObject * obj_p)
  {
  on_new_asset(obj_p);
  }

//---------------------------------------------------------------------------------------
//
void FSkookumScriptEditor::on_in_memory_asset_deleted(UObject * obj_p)
  {
  UBlueprint * blueprint_p = Cast<UBlueprint>(obj_p);
  if (blueprint_p)
    {
    get_runtime()->delete_class_script_files(blueprint_p->GeneratedClass);
    }
  }

//---------------------------------------------------------------------------------------
// Called when the map is done loading (load progress reaches 100%)
void FSkookumScriptEditor::on_map_opened(const FString & file_name, bool as_template)
  {
  // Generate all script files one more time to be sure
  get_runtime()->generate_all_class_script_files();

  // Let runtime know we are done opening a new map
  get_runtime()->on_editor_map_opened();
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditor::on_blueprint_compiled(UBlueprint * blueprint_p)
  {
  // Re-generate script files for this class as things might have changed
  get_runtime()->generate_class_script_files(blueprint_p->GeneratedClass, true);

  // Check that there's no dangling default constructor
  bool has_skookum_default_constructor = get_runtime()->has_skookum_default_constructor(blueprint_p->GeneratedClass);
  bool has_skookum_destructor = get_runtime()->has_skookum_destructor(blueprint_p->GeneratedClass);
  if (has_skookum_default_constructor || has_skookum_destructor)
    {
    // Determine if it has a SkookumScript component
    bool has_component = false;
    for (TFieldIterator<UProperty> property_it(blueprint_p->GeneratedClass, EFieldIteratorFlags::IncludeSuper); property_it; ++property_it)
      {
      UObjectPropertyBase * obj_property_p = Cast<UObjectPropertyBase>(*property_it);
      if (obj_property_p)
        {
        if (get_runtime()->is_skookum_component_class(obj_property_p->PropertyClass))
          {
          has_component = true;
          break;
          }
        }
      }

    // Has no Sk component, see if it is called by the graph somewhere
    if (!has_component)
      {
      bool has_constructor_node = false;
      bool has_destructor_node = false;
      TArray<class UEdGraph*> graph_array;
      blueprint_p->GetAllGraphs(graph_array);
      for (auto graph_iter = graph_array.CreateConstIterator(); graph_iter && !has_constructor_node; ++graph_iter)
        {
        UEdGraph * graph_p = *graph_iter;
        for (auto node_iter = graph_p->Nodes.CreateConstIterator(); node_iter; ++node_iter)
          {
          UK2Node_CallFunction * function_node_p = Cast<UK2Node_CallFunction>(*node_iter);
          if (function_node_p)
            {
            UFunction * function_p = function_node_p->GetTargetFunction();
            if (get_runtime()->is_skookum_blueprint_function(function_p))
              {
              if (function_p->GetName().EndsWith(TEXT("@ !"), ESearchCase::CaseSensitive))
                {
                has_constructor_node = true;
                if (has_destructor_node) break; // If we found both we can stop looking any further
                }
              else if (function_p->GetName().EndsWith(TEXT("@ !!"), ESearchCase::CaseSensitive))
                {
                has_destructor_node = true;
                if (has_constructor_node) break; // If we found both we can stop looking any further
                }
              }
            }
          }
        }

      if (has_skookum_default_constructor && !has_constructor_node)
        {
        FText title = FText::Format(FText::FromString(TEXT("SkookumScript default constructor never called in '{0}'")), FText::FromString(blueprint_p->GetName()));
        FMessageDialog::Open(
          EAppMsgType::Ok,
          FText::Format(FText::FromString(
          TEXT("The SkookumScript class '{0}' has a default constructor but the blueprint has neither a SkookumScript component nor is the default constructor invoked somewhere in an event graph. ")
          TEXT("That means the default constructor will never be called. ")
          TEXT("To fix this issue, either add a SkookumScript component to the Blueprint, or invoke the default constructor explicitely in its event graph.")), FText::FromString(blueprint_p->GetName())),
          &title);
        }
      else if (has_skookum_destructor && !has_destructor_node) // `else if` here so we won't show both dialogs, one is enough
        {
        FText title = FText::Format(FText::FromString(TEXT("SkookumScript destructor never called in '{0}'")), FText::FromString(blueprint_p->GetName()));
        FMessageDialog::Open(
          EAppMsgType::Ok,
          FText::Format(FText::FromString(
          TEXT("The SkookumScript class '{0}' has a destructor but the blueprint has neither a SkookumScript component nor is the destructor invoked somewhere in an event graph. ")
          TEXT("That means the destructor will never be called. ")
          TEXT("To fix this issue, either add a SkookumScript component to the Blueprint, or invoke the destructor explicitely in its event graph.")), FText::FromString(blueprint_p->GetName())),
          &title);
        }
      }
    }
  }

//---------------------------------------------------------------------------------------

void FSkookumScriptEditor::on_new_asset(UObject * obj_p)
  {
  UBlueprint * blueprint_p = Cast<UBlueprint>(obj_p);
  if (blueprint_p)
    {
    // Register callback so we know when this Blueprint has been compiled
    blueprint_p->OnCompiled().AddRaw(this, &FSkookumScriptEditor::on_blueprint_compiled);

    get_runtime()->generate_class_script_files(blueprint_p->GeneratedClass, true);
    get_runtime()->generate_used_class_script_files();
    }
  }

