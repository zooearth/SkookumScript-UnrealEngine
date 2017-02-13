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
// SkookumScript C++ library.
//
// Data structure for simplest type of object in language - instance of a
//             class without data members
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AObjReusePool.hpp>

//=======================================================================================
// SkDataInstance Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Constructor
A_INLINE SkDataInstance::SkDataInstance()
  {
  // Mark this instance as a data instance - allows for sanity checking down the road
  #if (SKOOKUM & SK_DEBUG)
    m_user_data.m_data.m_ptr[1] = ms_magic_marker;
  #endif
  }

//---------------------------------------------------------------------------------------
// Constructor
// Returns:    itself
// Arg         class_p - Class type for this instance
// Author(s):   Conan Reis
A_INLINE SkDataInstance::SkDataInstance(
  SkClass * class_p // = nullptr
  ) :
  SkInstance(class_p)
  {
  add_data_members();

  // Mark this instance as a data instance - allows for sanity checking down the road
  #if (SKOOKUM & SK_DEBUG)
    m_user_data.m_data.m_ptr[1] = ms_magic_marker;
  #endif
  }

//---------------------------------------------------------------------------------------

A_INLINE SkInstance * SkDataInstance::get_data_by_idx(uint32_t data_idx) const
  {
  #if (SKOOKUM & SK_DEBUG)
    if (m_user_data.m_data.m_ptr[1] != ms_magic_marker)
      {
      return on_magic_marker_mismatch(data_idx);
      }
  #endif

  return m_data[data_idx];
  }

//---------------------------------------------------------------------------------------
// 
A_INLINE void SkDataInstance::set_data_by_idx(uint32_t data_idx, SkInstance * obj_p)
  {
  #if (SKOOKUM & SK_DEBUG)
    if (m_user_data.m_data.m_ptr[1] != ms_magic_marker)
      {
      on_magic_marker_mismatch(data_idx);
      return;
      }
  #endif

  m_data.set_at(data_idx, *obj_p);
  }

//---------------------------------------------------------------------------------------
// Returns dynamic reference pool. Pool created first call and reused on successive calls.
// 
// #Notes
//   Uses Scott Meyers' tip "Make sure that objects are initialized before they're used"
//   from "Effective C++" [Item 47 in 1st & 2nd Editions and Item 4 in 3rd Edition]
//   This is instead of using a non-local static object for a singleton.
//   
// #Modifiers  static
// #Author(s)  Conan Reis
A_INLINE AObjReusePool<SkDataInstance> & SkDataInstance::get_pool()
  {
  static AObjReusePool<SkDataInstance> s_pool(SkookumScript::get_app_info()->get_pool_init_data_instance(), SkookumScript::get_app_info()->get_pool_incr_data_instance());

  return s_pool;
  }

//---------------------------------------------------------------------------------------
//  Retrieves an instance object from the dynamic pool and initializes it for
//              use.  This method should be used instead of 'new' because it prevents
//              unnecessary allocations by reusing previously allocated objects.
// Returns:     a dynamic SkInstance
// See:         pool_delete() 
// Notes:       To 'deallocate' an object that was retrieved with this method, use
//              'pool_delete()' rather than 'delete'.
// Modifiers:    static
// Author(s):    Conan Reis
A_INLINE SkDataInstance * SkDataInstance::new_instance(SkClass * class_p)
  {
  SkDataInstance * instance_p = get_pool().allocate();

  instance_p->m_class_p = class_p;
  instance_p->m_ref_count = 1u;
  instance_p->m_ptr_id = ++ms_ptr_id_prev;
  instance_p->add_data_members();

  // Mark this instance as a data instance - allows for sanity checking down the road
  #if (SKOOKUM & SK_DEBUG)
    instance_p->m_user_data.m_data.m_ptr[1] = ms_magic_marker;
  #endif

  return instance_p;
  }

