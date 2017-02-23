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
// Qualifier class - full qualification consisting of member name and owner class
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

//=======================================================================================
// Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
//  Default constructor
// Returns:     itself
// Arg          name - name of the object (Default ASymbol::ms_null)
// Arg          scope_p - optional class scope to use.  For most methods, a Qualifier
//              with nullptr as a scope implies that the topmost scoped object is being
//              referred to.  (Default nullptr)
// Notes:       A SkQualifier may be coerced from a ASymbol if only the name is needed.
// Author(s):    Conan Reis
A_INLINE SkQualifier::SkQualifier(
  const ASymbol & name,         // = ASymbol::ms_null
  SkClass *       scope_p,      // = nullptr
  int16_t         vtable_index  // = ms_invalid_vtable_index
  ) :
  SkNamedIndexed(name, vtable_index),
  m_scope_p(scope_p)
  {
  }

//---------------------------------------------------------------------------------------
//  Equal to operator
// Returns:     true if equal, false if not
// Arg          qual - other qualifier to compare against
// Notes:       This assumes that 'm_scope_p' for this instance is set and 'm_scope_p'
//              for 'qual' may or may not be set.  If 'm_scope_p' for 'qual' is not set,
//              only the name is compared.
// Author(s):    Conan Reis
A_INLINE bool SkQualifier::operator==(const SkQualifier & qual) const
  {
  return ((m_name == qual.m_name) && (qual.m_scope_p && (m_scope_p == qual.m_scope_p)));
  }

//---------------------------------------------------------------------------------------
//  Equal to ids operator
// Returns:     true if ids equal, false if not
// Arg          qual - other qualifier to compare against
// Author(s):    Conan Reis
A_INLINE bool SkQualifier::equal_ids(const SkQualifier & qual) const
  {
  return ((m_name == qual.m_name) && (m_scope_p == qual.m_scope_p));
  }

//---------------------------------------------------------------------------------------
//  Equal to ids operator
// Returns:     true if ids equal, false if not
// Arg          qual - other qualifier to compare against
// Author(s):    Conan Reis
A_INLINE bool SkQualifier::equal_ids_scope_name(const SkQualifier & qual) const
  {
  return ((m_scope_p == qual.m_scope_p) && (qual.m_name.is_null() || (m_name == qual.m_name)));
  }

//---------------------------------------------------------------------------------------
// Gets the class scope
// Returns:    Pointer to class scope or nullptr
// Author(s):   Conan Reis
A_INLINE SkClass * SkQualifier::get_scope() const
  {
  return m_scope_p;
  }

//---------------------------------------------------------------------------------------
//  Sets the class scope of the qualifier.
// Arg          scope_p - optional class scope to use.  nullptr indicates the topmost scope
//              when used with search / retrieval functions.  (Default nullptr)
// Author(s):    Conan Reis
A_INLINE void SkQualifier::set_scope(const SkClass * scope_p)
  {
  m_scope_p = const_cast<SkClass *>(scope_p);
  }

