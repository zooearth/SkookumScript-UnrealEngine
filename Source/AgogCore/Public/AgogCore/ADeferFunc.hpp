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
// Agog Labs C++ library.
//
// ADeferFunc class declaration header
//=======================================================================================

#pragma once 

//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AFunction.hpp>
#include <AgogCore/AMethod.hpp>
#include <AgogCore/APArray.hpp>


//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
class A_API ADeferFunc
  {
  public:

  // Class Methods

    static void post_func_obj(AFunctionBase * func_p);
    static void post_func(void (*function_f)());

    template<class _OwnerType>
      static void post_method(_OwnerType * owner_p, void (_OwnerType::* method_m)());

    static void invoke_deferred();


  // Class Data Members

    static APArrayFree<AFunctionBase> ms_deferred_funcs;

  };


//=======================================================================================
// Inline Functions
//=======================================================================================

//---------------------------------------------------------------------------------------
// Calls specified function object once invoke_deferred() is called - usually
//             at the end of a main loop or frame update.
//             This is convenient for some tasks that cannot occur immediately - which
//             is often true for events.  It allows the callstack to unwind and calls
//             the function at a less 'deep' location.
// Arg         func_p - pointer to function object to invoke at a later time.
// See:        ATimer
// Author(s):   Conan Reis
inline void ADeferFunc::post_func_obj(AFunctionBase * func_p)
  {
  ms_deferred_funcs.append(*func_p);
  }

//---------------------------------------------------------------------------------------
// Calls specified function once invoke_deferred() is called - usually at the
//             end of a main loop or frame update.
//             This is convenient for some tasks that cannot occur immediately - which
//             is often true for events.  It allows the callstack to unwind and calls
//             the function at a less 'deep' location.
// Arg         function_f - pointer to method to invoke at a later time.
// See:        ATimer
// Author(s):   Conan Reis
inline void ADeferFunc::post_func(void (*function_f)())
  {
  post_func_obj(new AFunction(function_f));
  }

//---------------------------------------------------------------------------------------
// Calls specified method once invoke_deferred() is called - usually at the
//             end of a main loop or frame update.
//             This is convenient for some tasks that cannot occur immediately - which
//             is often true for events.  It allows the callstack to unwind and calls
//             the function at a less 'deep' location.
// Arg         method_m - pointer to method to invoke at a later time.
// See:        ATimer
// Author(s):   Conan Reis
template<class _OwnerType>
inline void ADeferFunc::post_method(
  _OwnerType *        owner_p,
  void (_OwnerType::* method_m)())
  {
  post_func_obj(new AMethod<_OwnerType>(owner_p, method_m));
  }
