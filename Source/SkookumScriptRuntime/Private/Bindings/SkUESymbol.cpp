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
// SkookumScript Plugin for Unreal Engine 4
//
// Manages Symbols for SkookumScript
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include "../SkookumScriptRuntimePrivatePCH.h"

#include "SkUESymbol.hpp"

namespace SkUESymbol
  {

  //---------------------------------------------------------------------------------------
  // Assign values to the static symbols
  void initialize()
    {
    #define ASYM(_id)         ASYMBOL_ASSIGN(ASymbol, _id)
    #define ASYMX(_id, _str)  ASYMBOL_ASSIGN_STR(ASymbolX, _id, _str)
      SKUE_SYMBOLS
      SKUE_SYMBOLS_NAMED
    #undef ASYMX
    #undef ASYM
    }

  //---------------------------------------------------------------------------------------
  // Clean up static symbols
  void deinitialize()
    {
    #define ASYM(_id)         ASYMBOL_ASSIGN_NULL(ASymbol, _id)
    #define ASYMX(_id, _str)  ASYMBOL_ASSIGN_STR_NULL(ASymbolX, _id, _str)
      SKUE_SYMBOLS
      SKUE_SYMBOLS_NAMED
    #undef ASYMX
    #undef ASYM
    }

  }

//=======================================================================================
// Global Variables
//=======================================================================================

//---------------------------------------------------------------------------------------
// Define ASymbol constants

// Symbol Table to use in macros

#define ASYM(_id)         ASYMBOL_DEFINE_NULL(ASymbol, _id)
#define ASYMX(_id, _str)  ASYMBOL_DEFINE_STR_NULL(ASymbolX, _id, _str)
  SKUE_SYMBOLS
  SKUE_SYMBOLS_NAMED
#undef ASYMX
#undef ASYM

