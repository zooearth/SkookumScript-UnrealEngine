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
// SkookumScript Validated Object IDs
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkClass.hpp>
#include <SkookumScript/SkSymbol.hpp>


//=======================================================================================
// SkObjectIDBase Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Constructor
// 
// Returns:    itself
// Examples:   Called by SkParser
// See:        SkParser
// Author(s):   Conan Reis
A_INLINE SkObjectIDBase::SkObjectIDBase(
  SkClass * class_p,
  uint32_t flags
  ) :
  m_class_p(class_p),
  m_flags(flags)
  {
  }


#if (SKOOKUM & SK_COMPILED_IN)

//---------------------------------------------------------------------------------------
// Assignment from binary info - the identifier name must already have been processed.
// 
// Params:
//   binary_pp:
//     Pointer to address to read binary serialization info from and to increment
//     - previously filled using as_binary() or a similar mechanism
//     
// Notes:
//   Little error checking is done on the binary info as it assumed that it previously
//   validated upon input.
//   
//     Binary composition:
//               4 bytes - class name id
//               1 byte  - flags
//
// See:       as_binary()
// Author(s):   Conan Reis
A_INLINE void SkObjectIDBase::assign_binary(const void ** binary_pp)
  {
  // 4 bytes - class name id
  m_class_p = SkClass::from_binary_ref(binary_pp);

  // 1 byte - flags (only bother storing first 8 flags)
  m_flags = A_BYTE_STREAM_UI8_INC(binary_pp);
  }

#endif // (SKOOKUM & SK_COMPILED_IN)


//=======================================================================================
// SkObjectIDSymbol Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Constructor
// 
// Returns:    itself
// Examples:   Called by SkParser
// See:        SkParser
// Author(s):  Conan Reis
A_INLINE SkObjectIDSymbol::SkObjectIDSymbol(
  const ASymbol & name,
  SkClass *       class_p,
  uint32_t        flags
  ) :
  SkObjectIDBase(class_p, flags),
  m_name(name)
  {
  }


#if (SKOOKUM & SK_COMPILED_IN)

//---------------------------------------------------------------------------------------
// Constructor from binary info
// 
// Returns: itself
// 
// Params:
//   binary_pp:
//     Pointer to address to read binary serialization info from and to increment
//     - previously filled using as_binary() or a similar mechanism.e:        as_binary()
// Notes:
//   Little error checking is done on the binary info as it assumed that it previously
//   validated upon input.
//   
//     Binary composition:
//       n bytes - identifier name string
//       4 bytes - class name id
//       1 byte  - flags
//             
// Author(s):  Conan Reis
A_INLINE SkObjectIDSymbol::SkObjectIDSymbol(const void ** binary_pp)
  {
  // n bytes - identifier name string
  AString name(binary_pp);

  m_name = ASymbol::create(name, ATerm_short);

  // 4 bytes - class name id
  // 1 byte - flags (only bother storing first 8 flags)
  SkObjectIDBase::assign_binary(binary_pp);
  }

#endif // (SKOOKUM & SK_COMPILED_IN)

#if (SKOOKUM & SK_CODE_IN)

//---------------------------------------------------------------------------------------
// Ensures this is name id is valid for the associated class.
//
// #Author(s) Conan Reis
A_INLINE SkClass * SkObjectIDSymbol::validate(
  bool validate_deferred // = true
  )
  {
  return m_class_p->object_id_validate(this, validate_deferred);
  }

#endif // (SKOOKUM & SK_CODE_IN)


//---------------------------------------------------------------------------------------
// Get instance version of identifier name in the engine specific type
A_INLINE SkInstance * SkObjectIDSymbol::get_name_instance() const
  {
  return SkSymbol::new_instance(m_name);
  }
