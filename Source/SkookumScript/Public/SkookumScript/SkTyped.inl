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
// Typed name and typed data classes
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================


//=======================================================================================
// SkTypedName Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Constructor
// Returns:    itself
// Arg         name - name of the object (Default ASymbol::ms_null)
// Arg         type_p - class type to use.  The instance stored by this member must be of
//             this class or a subclass.
// Notes:      A SkTypedName may be coerced to or from a ASymbol if only the name is needed.
// Author(s):   Conan Reis
A_INLINE SkTypedName::SkTypedName(
  const ASymbol &         name,
  const SkClassDescBase * type_p
  ) :
  ANamed(name),
  m_type_p(const_cast<SkClassDescBase *>(type_p))
  {
  }

//---------------------------------------------------------------------------------------
// Copy constructor
// Returns:    itself
// Arg         source - Typed name to copy
// Notes:      May be coerced to or from a ASymbol if only the name is needed.
// Author(s):   Conan Reis
A_INLINE SkTypedName::SkTypedName(const SkTypedName & source) :
  ANamed(source),
  m_type_p(source.m_type_p)
  {
  }

//---------------------------------------------------------------------------------------
// Destructor
// Author(s):   Conan Reis
A_INLINE SkTypedName::~SkTypedName()
  {
  // Defined here to ensure compiler knows about SkClass details
  }

