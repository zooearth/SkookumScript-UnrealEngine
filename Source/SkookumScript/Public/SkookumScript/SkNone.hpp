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
// Special None class for the single nil instance.
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkClassBindingAbstract.hpp>


//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
class SK_API SkNone : public SkClassBindingAbstract<SkNone>, public SkInstanceUnreffed
  {
  public:
    
    SK_NEW_OPERATORS(SkNone);

    SkNone();

    static void       initialize();
    static SkClass *  get_class();

  };  // SkNone
