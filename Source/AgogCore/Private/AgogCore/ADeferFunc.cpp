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
// ADeferFunc class definition module
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AgogCore.hpp> // Always include AgogCore first (as some builds require a designated precompiled header)
#include <AgogCore/ADeferFunc.hpp>


//=======================================================================================
// Class Data
//=======================================================================================

APArrayFree<AFunctionBase> ADeferFunc::ms_deferred_funcs;


//=======================================================================================
// Class Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Invokes/calls any previously posted/deferred function objects.
// Notes:      Generally called end of a main loop or frame update.
// Modifiers:   static
// Author(s):   Conan Reis
void ADeferFunc::invoke_deferred()
  {
  uint func_count = ms_deferred_funcs.get_length();

  if (func_count)
    {
    // The functions are called in the order that they were posted
    AFunctionBase ** funcs_pp     = ms_deferred_funcs.get_array();
    AFunctionBase ** funcs_end_pp = funcs_pp + func_count;

    for (; funcs_pp < funcs_end_pp; funcs_pp++)
      {
      (*funcs_pp)->invoke();

      delete *funcs_pp;
      }

    // Only remove the number of function objects invoked rather than just emptying the
    // array since new function objects may have been posted.
    ms_deferred_funcs.remove_all(0u, func_count);
    }
  }
