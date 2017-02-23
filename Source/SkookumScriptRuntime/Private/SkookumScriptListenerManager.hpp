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
// Factory/manager class for USkookumScriptListener objects
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "SkookumScriptRuntimePrivatePCH.h"

#include "SkookumScriptListener.h"

#include <AgogCore/APArray.hpp>
#include <SkookumScript/SkInstance.hpp>

//=======================================================================================
// Global Defines / Macros
//=======================================================================================

//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// Keep track of USkookumScriptListener instances
class SkookumScriptListenerManager
  {
  public:

    static SkookumScriptListenerManager *   get_singleton();

    // Methods

    SkookumScriptListenerManager(uint32_t pool_init, uint32_t pool_incr);
    ~SkookumScriptListenerManager();

    USkookumScriptListener *                alloc_listener(UObject * obj_p, SkInvokedCoroutine * coro_p, USkookumScriptListener::tUnregisterCallback callback_p);
    void                                    free_listener(USkookumScriptListener * listener_p);

    USkookumScriptListener::EventInfo *     alloc_event();
    void                                    free_event(USkookumScriptListener::EventInfo * event_p, uint32_t num_arguments_to_free);

  protected:

    typedef APArray<USkookumScriptListener> tObjPool;
    typedef AObjReusePool<USkookumScriptListener::EventInfo> tEventPool;

    void              grow_inactive_list(uint32_t pool_incr);

    tObjPool          m_inactive_list;
    tObjPool          m_active_list;
    uint32_t          m_pool_incr;

    tEventPool        m_event_pool;

  }; // SkookumScriptListenerManager

//=======================================================================================
// Inline Functions
//=======================================================================================

//---------------------------------------------------------------------------------------

inline USkookumScriptListener::EventInfo * SkookumScriptListenerManager::alloc_event()
  {
  return m_event_pool.allocate();
  }

//---------------------------------------------------------------------------------------
            
inline void SkookumScriptListenerManager::free_event(USkookumScriptListener::EventInfo * event_p, uint32_t num_arguments_to_free)
  {
  for (uint32_t i = 0; i < num_arguments_to_free; ++i)
    {
    event_p->m_argument_p[i]->dereference();
    }
  m_event_pool.recycle(event_p);
  }

