//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript Remote Client
// 
// Author: Conan Reis
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "../SkookumScriptRuntimePrivatePCH.h"

#include "IPluginManager.h"
#include "Networking.h"
#include "ISkookumScriptRuntime.h"
#include <AgogCore/ADatum.hpp>
#include <AgogCore/AMath.hpp>
#include <SkookumScript/SkRemoteRuntimeBase.hpp>

//=======================================================================================
// Global Structures
//=======================================================================================

#if (SKOOKUM & SK_DEBUG)
//#ifdef SKOOKUM_REMOTE
  // Enable remote SkookumIDE for debugging in the SkookumScript Unreal plug-in
  #define SKOOKUM_REMOTE_UNREAL
#endif

#ifdef SKOOKUM_REMOTE_UNREAL

class FSkookumScriptRuntimeGenerator;
  
//---------------------------------------------------------------------------------------
// Communication commands that are specific to the SkookumIDE.
class SkUERemote : public SkRemoteRuntimeBase
  {
  public:

  // Common Methods

    SkUERemote(FSkookumScriptRuntimeGenerator * runtime_generator_p);
    ~SkUERemote();

    void                      process_incoming();

    TSharedPtr<FInternetAddr> get_ip_address_local();
    TSharedPtr<FInternetAddr> get_ip_address_ide();

    virtual bool              is_connected() const override;
    virtual void              set_mode(eSkLocale mode) override;

    bool                      is_load_compiled_binaries_requested() const { return m_remote_binaries == CompiledState_fresh; }
    void                      clear_load_compiled_binaries_requested()    { m_remote_binaries = CompiledState_unknown; }
    bool                      is_compiled_binaries_have_errors() const    { return m_remote_binaries == CompiledState_errors; }

    //---------------------------------------------------------------------------------------
    // Determines amount of time elapsed time in seconds (from some consistent start time at
    // or before its first call like: start of system, start of app launch, etc.)
    //
    // Notes:
    //   Used to time system / debug wait intervals.
    //   Should only be used as a difference between an earlier call.
    //   The double has sufficient precision such that it can be used to represent elapsed
    //   milliseconds without error until 285,421 years have passed..
    virtual double get_elapsed_seconds() override;

    //---------------------------------------------------------------------------------------
    // Supply information about current project
    // 
    // See: SkRemoteRuntimeBase::cmd_compiled_state()
    virtual void get_project_info(SkProjectInfo * out_project_info_p) override;

    //---------------------------------------------------------------------------------------
    // Brute force spawn of remote IDE. Called when connect attempts don't work and it is
    // assumed that the remote IDE is not running.
    // 
    // Returns:  true if remote IDE started false if not (due to some error, etc.)
    virtual bool spawn_remote_ide() override;

    //---------------------------------------------------------------------------------------
    // Blocking 1/10 second (100 millisecond) wait for current thread/message handle as
    // appropriate for current platform and project.
    //
    // Notes:
    //   Should also update SkRemoteRuntimeBase derived objects if they aren't updated on
    //   one or more separate threads. This can be done by calling concurrent process updates
    //   like message handlers or by calling a custom SkRemoteRuntimeBase object's update
    //   method directly.
    virtual void wait_for_update() override;

    // Callbacks

    void set_editor_interface(ISkookumScriptRuntimeEditorInterface * editor_interface_p);

    // Commands

  protected:

    AString                   get_socket_str(const FInternetAddr & addr);
    AString                   get_socket_str();

  // Events

    virtual void              on_cmd_send(const ADatum & datum) override;
    virtual void              on_cmd_make_editable() override;
    virtual void              on_cmd_freshen_compiled_reply(eCompiledState state) override;
    virtual void              on_class_updated(SkClass * class_p) override;

  // Data Members

    FSocket *     m_socket_p;

    // Datum that is filled when data is received
    ADatum        m_data_in;

    // Data byte index point - ADef_uint32 when not in progress
    uint32_t      m_data_idx;

    // Editor interface so we can notify it about interesting events
    ISkookumScriptRuntimeEditorInterface * m_editor_interface_p;

    // Generator so we can get project information
    FSkookumScriptRuntimeGenerator * m_runtime_generator_p;

  };  // SkUERemote

#endif  // SKOOKUM_REMOTE_UNREAL

