//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// Invokable parameters & body class
// Author(s):   Conan Reis
// Notes:          
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
  uint32_t        invoked_data_array_size
  ) :
  SkQualifier(name, scope_p),
  m_invoked_data_array_size((uint16_t)invoked_data_array_size)
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
  uint32_t        invoked_data_array_size
  ) :
  SkQualifier(name, scope_p),
  m_params_p(params_p),
  m_invoked_data_array_size((uint16_t)invoked_data_array_size)
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
  uint32_t invoked_data_array_size
  ) :
  SkQualifier(name),
  m_params_p(SkParameters::get_or_create(result_type_p, param_p))
  {}

