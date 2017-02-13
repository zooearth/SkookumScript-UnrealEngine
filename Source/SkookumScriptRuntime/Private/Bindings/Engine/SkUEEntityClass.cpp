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
// Additional bindings for the EntityClass (= UClass) class 
//=======================================================================================

//=======================================================================================
// Includes
//=======================================================================================

#include "../../SkookumScriptRuntimePrivatePCH.h"
#include "SkUEEntityClass.hpp"
#include "../SkUEUtils.hpp"

//---------------------------------------------------------------------------------------

namespace SkUEEntityClass_Impl
  {

  //=======================================================================================
  // Methods
  //=======================================================================================

  //---------------------------------------------------------------------------------------
  // # Skookum:   UClass@String() String
  // # Author(s): Markus Breyer
  static void mthd_String(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    if (result_pp) // Do nothing if result not desired
      {
      UClass * uclass_p = scope_p->this_as<SkUEEntityClass>();
      AString uclass_name = FStringToAString(uclass_p->GetName());

      AString str(nullptr, 3u + uclass_name.get_length(), 0u);
      str.append('(');
      str.append(uclass_name);
      str.append(')');
      *result_pp = SkString::new_instance(str);
      }
    }

  static const SkClass::MethodInitializerFunc methods_i2[] =
    {
      { "String", mthd_String },
    };

  } // SkUEEntityClass_Impl

//---------------------------------------------------------------------------------------
void SkUEEntityClass_Ext::register_bindings()
  {
  ms_class_p->register_method_func_bulk(SkUEEntityClass_Impl::methods_i2, A_COUNT_OF(SkUEEntityClass_Impl::methods_i2), SkBindFlag_instance_no_rebind);
  }
