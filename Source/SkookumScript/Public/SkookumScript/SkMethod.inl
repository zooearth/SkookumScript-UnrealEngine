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
// Method parameters & body classes
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

//=======================================================================================
// SkMethodFunc Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Constructor
// Returns:    itself
// Arg         name - identifier name for the method
// Arg         scope_p - class scope to use
// Arg         params_p - parameters object to take over contents of
// Arg         atomic_f - C++ static / class member function to call when the method is
//             invoked / called - nullptr if supplied at a later time.  (Default nullptr)
// See:        Transfer constructor of SkParameters.
// Author(s):   Conan Reis
A_INLINE SkMethodFunc::SkMethodFunc(
  const ASymbol & name,
  SkClass *       scope_p,
  SkParameters *  params_p,
  uint32_t        annotation_flags,
  tSkMethodFunc   atomic_f // = nullptr
  ) :
  SkMethodBase(name, scope_p, params_p, params_p->get_arg_count_total(), annotation_flags),
  m_atomic_f(atomic_f)
  {
  }


//=======================================================================================
// SkMethodMthd Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Constructor
// Returns:    itself
// Arg         name - identifier name for the method
// Arg         scope_p - class scope to use
// Arg         params_p - parameters object to take over contents of
// Arg         atomic_m - C++ method to call when the method is invoked / called - nullptr
//             if supplied at a later time.  (Default nullptr)
// See:        Transfer constructor of SkParameters.
// Author(s):   Conan Reis
A_INLINE SkMethodMthd::SkMethodMthd(
  const ASymbol & name,
  SkClass *       scope_p,
  SkParameters *  params_p,
  uint32_t        annotation_flags,
  tSkMethodMthd   atomic_m // = nullptr
  ) :
  SkMethodBase(name, scope_p, params_p, params_p->get_arg_count_total(), annotation_flags),
  m_atomic_m(atomic_m)
  {
  }

//=======================================================================================
// SkMethod Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Overwrite the expression, written as a hack to allow external Skookum commands
//			   to keep their code after it has run
// Arg         expr_p - pointer to code (can be nullptr)
// Notes:      Function so we can set the expression to nullptr so that it doesn't get
//             deleted by the method destructor intended only to be used as a hack to get
//             the external Skookum calls to work correctly
// Author(s):   Richard Orth
A_INLINE void SkMethod::replace_expression(SkExpressionBase * expr_p)
  {
  m_expr_p = expr_p;
  }

