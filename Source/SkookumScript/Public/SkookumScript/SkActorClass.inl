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
// Data structure for actor class descriptors and actor class objects
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkDebug.hpp>


//=======================================================================================
// Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
//  Constructor
// Returns:     itself
// Arg          name - Name of the class.  It should follow the form of "ClassName"
//              uppercase {alphanumeric}
// Arg          superclass_p - pointer to superclass of this class.  Only "Object" should
//              have no superclass.
// Author(s):    Conan Reis
A_INLINE SkActorClass::SkActorClass(
  const ASymbol & name,             // = ASymbol::ms_null
  SkClass *       superclass_p,     // = nullptr
  uint32_t        flags,            // = Flag__default_actor
  uint32_t        annotation_flags  // = 0
  ) :
  SkClass(name, superclass_p, flags, annotation_flags)
  {
  }

//---------------------------------------------------------------------------------------
// Appends an actor instance of this class to the instance list of this class
// Arg         actor - actor instance to append
// Examples:   called in SkActor::SkActor()
// Author(s):   Conan Reis
A_INLINE void SkActorClass::append_instance(const SkActor & actor)
  {
  //A_DPRINT(A_SOURCE_STR " SkActorClass::append_instance('%s')\n", actor.get_name_cstr_dbg());

  if (actor.get_name() != ASymbol::get_null())
    {
    //if (m_instances.find(actor.get_name()))
    //  {
    //  A_DPRINT(A_SOURCE_STR " SkActorClass::append_instance('%s') - dupe found!\n", actor.get_name_cstr_dbg());
    //  }

    m_instances.append(actor);

    if (m_superclass_p->is_builtin_actor_class())
      {
      static_cast<SkActorClass *>(m_superclass_p)->append_instance(actor);
      }
    }
  }

//---------------------------------------------------------------------------------------
// Removes an actor instance of this class from the instance list of this class.
// 
// Params:  
//   actor: actor instance to remove
//   term: if ATerm_short then it is only a temporary removal for something like a rename
//   
// Examples:   called in SkActor::~SkActor() and SkActor:rename()
// Author(s):   Conan Reis
A_INLINE void SkActorClass::remove_instance(
  const SkActor & actor,
  eATerm          term // = ATerm_long
  )
  {
  //A_DPRINT(A_SOURCE_STR " SkActorClass::remove_instance('%s')\n", actor.get_name_cstr_dbg());

  if (actor.get_name() != ASymbol::get_null())
    {
    m_instances.remove_elem(actor);

    if (m_superclass_p->is_builtin_actor_class())
      {
      static_cast<SkActorClass *>(m_superclass_p)->remove_instance(actor);
      }

    if ((term == ATerm_long) && m_instances.is_empty())
      {
      m_instances.compact();

      // If there was a deferred unload and there are no more instances, do the unload now
      if (m_flags & Flag_demand_unload)
        {
        demand_unload();
        }
      }
    }
  }

