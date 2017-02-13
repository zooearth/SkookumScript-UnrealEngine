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
// AFunctionBase class declaration header
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AgogCore.hpp>


//=======================================================================================
// Global Macros / Defines
//=======================================================================================

// Returns a new dynamic copy of a class instance that is or is derived from the class
// specified by _BaseClass.  Used virtually when a dynamic copy is needed, but the class
// is unknown.
#define AFUNC_COPY_NEW_DEF(_BaseClass) public: virtual _BaseClass * copy_new() const;

//---------------------------------------------------------------------------------------
//  Returns a new dynamic copy of itself.  Used virtually when a dynamic copy
//              is needed, but the class is unknown.
// Returns:     new dynamic copy of itself
// Examples:     _BaseClass * new_obj_p = some_obj.copy_new();
// Notes:       The class indicated by _FuncClass must have a copy constructor.
// Author(s):    Conan Reis
#define AFUNC_COPY_NEW_DECL(_FuncClass, _BaseClass) \
  _BaseClass * _FuncClass::copy_new() const \
    { \
    _BaseClass * func_obj_p = (new _FuncClass(*this)); \
    A_VERIFY_MEMORY(func_obj_p != nullptr, _FuncClass); \
    return func_obj_p; \
    } 


//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// Notes     This is a function callback object.  This class is used in the place of a
//           function argument since it can also use data included with it in any
//           evaluations that take place (if derived).  The fact that the data that the
//           function acts upon is stored with this functional object is especially
//           important for concurrent processing in that it does not rely on global data
//           and thus may operate safely in more than one thread of execution
//           simultaneously.
// Parents  
// Uses     
// InLib     AgogCore/AgogCore.lib
// Examples:    
// Author(s) Conan Reis
class A_API AFunctionBase
  {
  public:

    A_NEW_OPERATORS(AFunctionBase);

  // Common Methods

    virtual ~AFunctionBase();

  // Modifying Methods

    virtual void invoke() = 0;

  // Non-Modifying Methods

    virtual AFunctionBase * copy_new() const = 0;

  };  // AFunctionBase


//=======================================================================================
// Inline Functions
//=======================================================================================

