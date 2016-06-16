//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Main entry point for the SkookumScript runtime plugin
// 
// Author: Markus Breyer
//=======================================================================================

#pragma once

#include "ModuleManager.h"

class SkClass;
class UClass;
class UBlueprint;

//---------------------------------------------------------------------------------------
// Interface class for the runtime plugin to call the editor plugin
class ISkookumScriptRuntimeEditorInterface
  {
  public:

  #if WITH_EDITOR

    virtual void  recompile_blueprints_with_errors() const = 0;
    virtual void  on_class_updated(UClass * ue_class_p) = 0;
    virtual bool  check_out_file(const FString & file_path) const = 0;

  #endif

  };


//---------------------------------------------------------------------------------------
// The public interface to this module
class ISkookumScriptRuntime : public IModuleInterface
  {
  public:

    // Methods

    virtual bool  is_skookum_disabled() const = 0;
    virtual void  startup_skookum() = 0;
    virtual bool  is_freshen_binaries_pending() const = 0;

    #if WITH_EDITOR

      virtual void  set_editor_interface(ISkookumScriptRuntimeEditorInterface * editor_interface_p) = 0;

      virtual void  on_editor_map_opened() = 0;
      virtual void  show_ide(const FString & focus_class_name, const FString & focus_member_name, bool is_data_member, bool is_class_member) = 0;
      virtual void  freshen_compiled_binaries_if_have_errors() = 0;

      virtual bool  has_skookum_default_constructor(UClass * class_p) const = 0;
      virtual bool  has_skookum_destructor(UClass * class_p) const = 0;
      virtual bool  is_skookum_component_class(UClass * class_p) const = 0;
      virtual bool  is_skookum_blueprint_function(UFunction * function_p) const = 0;
      virtual bool  is_skookum_blueprint_event(UFunction * function_p) const = 0;

      virtual void  generate_class_script_files(UClass * ue_class_p, bool generate_data) = 0;
      virtual void  generate_used_class_script_files() = 0;
      virtual void  generate_all_class_script_files() = 0;
      virtual void  rename_class_script_files(UClass * ue_class_p, const FString & old_class_name) = 0;
      virtual void  rename_class_script_files(UClass * ue_class_p, const FString & old_class_name, const FString & new_class_name) = 0;
      virtual void  delete_class_script_files(UClass * ue_class_p) = 0;

    #endif

  };

