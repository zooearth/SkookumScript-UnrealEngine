//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// The "Brain" class - holds class hierarchy and other misc. objects that do
//             not have an obvious home elsewhere.
// Author(s):   Conan Reis
// Notes:          
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/ASymbol.hpp>
#include <SkookumScript/Sk.hpp>


//=======================================================================================
// Global Structures
//=======================================================================================

enum eSkLoadStatus
  {
  SkLoadStatus_ok,
  SkLoadStatus_stale,
  SkLoadStatus_runtime_old,
  SkLoadStatus_not_found,
  SkLoadStatus_debug_info_not_found
  };

//---------------------------------------------------------------------------------------
// Stores binary info memory pointer - used with the SkRuntimeBase get_binary*() and
// release_binary() methods.
// If needed, subclass this class for subclasses of SkRuntimeBase to add user /
// platform / project specific handle / pointer to manage the memory - for example a file
// handle.
struct SkBinaryHandle
  {
  // Public Data

    // Pointer to memory this handle represents
    void * m_binary_p;

    // Size of memory in bytes
    // [Could be 64-bit though Skookum binary files should not need to be over 4GB.]
    uint32_t m_size;

  // Public Methods

            SkBinaryHandle(void * binary_p = nullptr, uint32_t size = 0) : m_binary_p(binary_p), m_size(size) {}
    virtual ~SkBinaryHandle() {}
  };

//---------------------------------------------------------------------------------------
// Abstract base object for platform/OS specific / IO-based functions needed by Skookum
// for debugging and other tasks.  Derive a subclass and instantiate it prior to using
// Skookum.  Each method should be overridden as needed.  
// 
// Other areas that have external function hooks:
//   - A_BREAK
//   - memory allocate/deallocate, etc.
//   - SkRemoteBase socket functionality + spawn remote IDE
//   - SkDebug execution hooks
class SK_API SkRuntimeBase
  {
  public:

  // Class Data Members

    static SkRuntimeBase * ms_singleton_p;

  // Methods

    SkRuntimeBase();
    ~SkRuntimeBase();

    // Binary Serialization / Loading Overrides

      virtual bool             is_binary_hierarchy_existing() = 0;
      virtual void             on_binary_hierarchy_path_changed() = 0; // Called if the binaries are moved around on disk
      virtual SkBinaryHandle * get_binary_hierarchy() = 0;
      virtual SkBinaryHandle * get_binary_class_group(const SkClass & cls) = 0;
    #if defined(A_SYMBOL_STR_DB_AGOG) // Only needed when a symbol table is desired
      virtual SkBinaryHandle * get_binary_symbol_table() = 0;
    #endif
      virtual void             release_binary(SkBinaryHandle * handle_p) = 0;

    // Script Loading / Binding

      eSkLoadStatus load_compiled_hierarchy();
      virtual void  load_compiled_class_group(SkClass * class_p);
      void          load_compiled_class_group_all();
      virtual void  on_bind_routines();
      virtual void  on_pre_deinitialize_sim();

    // Flow Methods

      // Update methods (in order of preference)
      // - just use *one* of these (the most convenient version) once an update

      static void update(uint64_t sim_ticks, f64 sim_time, f32 sim_delta);
      static void update(uint64_t sim_ticks);
      static void update(float sim_delta);

  protected:

    // Internal class methods

      static void bind_routines();
  };
