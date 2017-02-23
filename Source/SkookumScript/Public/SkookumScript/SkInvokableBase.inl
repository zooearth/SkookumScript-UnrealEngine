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
// Invokable parameters & body class
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkDebug.hpp>

//=======================================================================================
// Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Default constructor - no name, class or parameters
// Returns:    itself
// Author(s):   Conan Reis
A_INLINE SkInvokableBase::SkInvokableBase()
  : m_invoked_data_array_size(0)
  , m_user_data(0)
  , m_annotation_flags(0)
  {
  SK_ASSERTX(false, "m_invoked_data_array_size = ??");
  }

//---------------------------------------------------------------------------------------
// Constructor without parameters
// Returns:    itself
// Arg         name - name of the object
// Arg         scope_p - class scope to use
// Arg         params - parameters object to take over contents of
// See:        Transfer constructor of SkParameters.
// Author(s):   Conan Reis
A_INLINE SkInvokableBase::SkInvokableBase(
  const ASymbol & name,
  SkClass *       scope_p,
  uint32_t        invoked_data_array_size,
  uint32_t        annotation_flags
  ) :
  SkQualifier(name, scope_p),
  m_invoked_data_array_size((uint16_t)invoked_data_array_size),
  m_user_data(0),
  m_annotation_flags(annotation_flags)
  {
  }

//---------------------------------------------------------------------------------------
// Constructor with name, class and parameters
// Returns:    itself
// Arg         name - name of the object
// Arg         scope_p - class scope to use
// Arg         params_p - parameters use - see SkParameters::get_or_create()
// Author(s):   Conan Reis
A_INLINE SkInvokableBase::SkInvokableBase(
  const ASymbol & name,
  SkClass *       scope_p,
  SkParameters *  params_p,
  uint32_t        invoked_data_array_size,
  uint32_t        annotation_flags
  ) :
  SkQualifier(name, scope_p),
  m_params_p(params_p),
  m_invoked_data_array_size((uint16_t)invoked_data_array_size),
  m_user_data(0),
  m_annotation_flags(annotation_flags)
  {
  }

//---------------------------------------------------------------------------------------
// #Description
//   Constructor with name, class and return type and optional parameter
//
// #Author(s) Conan Reis
A_INLINE SkInvokableBase::SkInvokableBase(
  // Name of invokable
  const ASymbol & name,
  // Result class type for parameter interface
  SkClassDescBase * result_type_p,
  // Optional single unary parameter - No parameter if nullptr.
  SkParameterBase * param_p,
  // Needed size of data array in invoked method/coroutine at runtime
  uint32_t invoked_data_array_size,
  // Which annotations are present
  uint32_t annotation_flags
  ) :
  SkQualifier(name),
  m_params_p(SkParameters::get_or_create(result_type_p, param_p)),
  m_invoked_data_array_size((uint16_t)invoked_data_array_size),
  m_user_data(0),
  m_annotation_flags(annotation_flags)
  {}

