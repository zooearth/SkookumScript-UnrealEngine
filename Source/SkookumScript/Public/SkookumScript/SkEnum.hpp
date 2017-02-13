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
// SkookumScript atomic Enumeration class
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkClassBinding.hpp>

//---------------------------------------------------------------------------------------
// SkookumScript atomic Enumeration class
class SK_API SkEnum : public SkClassBindingSimpleZero<SkEnum, SkEnumType>
  {
  public:

    // Class Methods

    static SkInstance *   new_instance(SkEnumType value, SkClass * enum_class_p);
    static ANamed *       get_class_data_enum_name(SkClass * enum_class_p, SkEnumType enum_value);

    static void           register_bindings();
    static SkClass *      get_class();

  };

//=======================================================================================
// Inline Methods
//=======================================================================================

inline SkInstance * SkEnum::new_instance(SkEnumType value, SkClass * enum_class_p)
  {
  SkInstance * instance_p = SkInstance::new_instance(enum_class_p);

  instance_p->construct<SkEnum>(value);

  return instance_p;
  }
