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
//
// SkookumScript Runtime Hooks for Unreal - Input/Output Init/Update/Deinit Manager
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "../SkookumScriptListenerManager.hpp"
#include "SkUEBlueprintInterface.hpp"

#include "Platform.h"  // Set up base types, etc for the platform

#include <SkookumScript/SkRuntimeBase.hpp>
#include <SkookumScript/SkParser.hpp>

//---------------------------------------------------------------------------------------

class ISkookumScriptRuntimeEditorInterface;
class SkUEBindingsInterface;

//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// SkookumScript Runtime Hooks for Unreal
// - Input/Output Init/Update/Deinit Manager
class SkUERuntime :
  public SkParserCustomBase,  // Since the runtime may need to parse text code
  public SkRuntimeBase
  {
  public:

    static SkUERuntime * get_singleton() { return static_cast<SkUERuntime *>(SkRuntimeBase::ms_singleton_p); }

  // Methods

    SkUERuntime();
    ~SkUERuntime() {}

    void startup();
    void shutdown();

    void ensure_static_ue_types_registered();

    // Script Loading / Binding

      const FString & get_compiled_path() const;
      bool            content_file_exists(const TCHAR * file_name_p, FString * folder_path_p) const;

      bool load_and_bind_compiled_scripts(bool is_hot_reload = false, bool ensure_atomics = true, SkClass ** ignore_classes_pp = nullptr, uint32_t ignore_count = 0u);
      bool load_compiled_scripts();
      void bind_compiled_scripts(bool is_hot_reload = false, bool ensure_atomics = true, SkClass ** ignore_classes_pp = nullptr, uint32_t ignore_count = 0u);

      void sync_all_blueprint_bindings_from_binary();
      void expose_all_blueprint_bindings(bool is_final);

    // Overridden from SkRuntimeBase

      // Binary Serialization / Loading Overrides

        virtual bool             is_binary_hierarchy_existing() override;
        virtual void             on_binary_hierarchy_path_changed() override;
        virtual SkBinaryHandle * get_binary_hierarchy() override;
        virtual SkBinaryHandle * get_binary_class_group(const SkClass & cls) override;
        virtual void             release_binary(SkBinaryHandle * handle_p) override;

        #if (SKOOKUM & SK_COMPILED_IN)
          virtual SkObjectIDBase * object_id_binary_new(const void ** binary_pp) override;
        #endif

        #if (SKOOKUM & SK_CODE_IN)
          virtual SkObjectIDBase * object_id_new(const ASymbol & name, SkClass * class_p, uint32_t flags) override;
          virtual SkClass *        object_id_name_class() override;
        #endif

        #if defined(A_SYMBOL_STR_DB_AGOG)  
          virtual SkBinaryHandle * get_binary_symbol_table() override;
        #endif

      // Flow Methods

        virtual void on_bind_routines() override;
        virtual void on_initialization_level_changed(SkookumScript::eInitializationLevel from_level, SkookumScript::eInitializationLevel to_level);

      // Accessors

        bool                                   is_initialized() const             { return m_is_initialized; }
        bool                                   is_compiled_scripts_bound() const  { return m_is_compiled_scripts_bound; }
        bool                                   is_compiled_scripts_loaded() const { return m_is_compiled_scripts_loaded; }
        bool                                   have_game_module() const           { return m_have_game_module; }

        SkookumScriptListenerManager *         get_listener_manager()                 { return &m_listener_manager; }
        SkUEBlueprintInterface *               get_blueprint_interface()              { return &m_blueprint_interface; }
        const SkUEBlueprintInterface *         get_blueprint_interface() const        { return &m_blueprint_interface; }
        ISkookumScriptRuntimeEditorInterface * get_editor_interface() const           { return m_editor_interface_p; }
        SkUEBindingsInterface *                get_project_generated_bindings() const { return m_project_generated_bindings_p; }

        void                                   set_project_generated_bindings(SkUEBindingsInterface * game_bindings_p);
        void                                   set_editor_interface(ISkookumScriptRuntimeEditorInterface * editor_interface_p)  { m_editor_interface_p = editor_interface_p; }

  protected:

    // Data Members

      bool                m_is_initialized;
      bool                m_is_compiled_scripts_loaded; // If compiled binaries have ever been loaded
      bool                m_is_compiled_scripts_bound;  // If on_bind_routines() has been called at least once
      bool                m_have_game_module; // If set_project_generated_bindings() was called at least once

      mutable bool        m_compiled_file_b;
      mutable FString     m_compiled_path;

      SkookumScriptListenerManager m_listener_manager;
      SkUEBlueprintInterface       m_blueprint_interface;

      SkUEBindingsInterface *                 m_project_generated_bindings_p;
      ISkookumScriptRuntimeEditorInterface *  m_editor_interface_p;

  };  // SkUERuntime

