//=======================================================================================
// SkookumScript Unreal Engine Runtime Script Generator
// Copyright (c) 2016 Agog Labs Inc. All rights reserved.
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

#include "../../SkookumScriptGenerator/Private/SkookumScriptGeneratorBase.h" // Need this even in cooked builds

#if WITH_EDITORONLY_DATA

//---------------------------------------------------------------------------------------

class ISkookumScriptRuntimeInterface
  {
  public:

    // Types

    enum eChangeType
      {
      ChangeType_created,
      ChangeType_modified,
      ChangeType_deleted
      };

    // Methods

    virtual bool  is_static_class_known_to_skookum(UClass * class_p) const = 0;
    virtual bool  is_static_struct_known_to_skookum(UStruct * struct_p) const = 0;
    virtual bool  is_static_enum_known_to_skookum(UEnum * enum_p) const = 0;

    virtual void  on_class_scripts_changed_by_generator(const FString & class_name, eChangeType change_type) = 0;
    virtual bool  check_out_file(const FString & file_path) const = 0;

  };

//---------------------------------------------------------------------------------------

class FSkookumScriptRuntimeGenerator : public FSkookumScriptGeneratorBase
  {
  public:

    // Methods

                  FSkookumScriptRuntimeGenerator(ISkookumScriptRuntimeInterface * runtime_interface_p);
                  ~FSkookumScriptRuntimeGenerator();

    FString       get_project_path();
    FString       get_default_project_path();
    int32         get_overlay_path_depth() const;
    void          generate_all_class_script_files();
    FString       make_project_editable();
    UBlueprint *  load_blueprint_asset(const FString & class_path, bool * sk_class_deleted_p);

    bool          is_property_type_supported_and_known(UProperty * property_p) const;
    void          generate_class_script_files(UClass * ue_class_p, bool generate_data, bool check_if_reparented);
    void          rename_class_script_files(UClass * ue_class_p, const FString & old_class_name);
    void          rename_class_script_files(UClass * ue_class_p, const FString & old_class_name, const FString & new_class_name);
    void          delete_class_script_files(UClass * ue_class_p);
    void          generate_used_class_script_files();

  protected:

    void          initialize_paths();

    // Data members

    ISkookumScriptRuntimeInterface * m_runtime_interface_p;

    FString       m_project_path;
    FString       m_default_project_path;

    FString       m_package_name_key;
    FString       m_package_path_key;

    const TCHAR * m_editable_ini_settings_p; // ini file settings to describe that a project is not editable

  };

#endif // WITH_EDITORONLY_DATA