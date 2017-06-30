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
// SkookumScript MulticastDelegate (= FMulticastScriptDelegate) class
//=======================================================================================

//=======================================================================================
// Includes
//=======================================================================================

#include "../../SkookumScriptRuntimePrivatePCH.h"

#include "SkUEMulticastDelegate.hpp"
#include "Bindings/SkUEUtils.hpp"
#include "Bindings/SkUEReflectionManager.hpp"

#include "UObject/Object.h"

#include <SkookumScript/SkBoolean.hpp>
#include <SkookumScript/SkClosure.hpp>
#include <SkookumScript/SkParameters.hpp>
#include <SkookumScript/SkString.hpp>

//=======================================================================================
// Method Definitions
//=======================================================================================

namespace SkUEMulticastDelegate_Impl
  {

  //---------------------------------------------------------------------------------------
  // # Skookum:   MulticastDelegate@String() String
  // # Author(s): Markus Breyer
  static void mthd_String(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    // Do nothing if result not desired
    if (result_pp)
      {
      const FMulticastScriptDelegate & script_delegate = scope_p->this_as<SkUEMulticastDelegate>();
      *result_pp = SkString::new_instance(FStringToAString(script_delegate.ToString<UObject>()));
      }
    }
    
  // Array listing all the above methods
  static const SkClass::MethodInitializerFunc methods_i[] =
    {
      { "String", mthd_String },
    };

} // namespace

//---------------------------------------------------------------------------------------
void SkUEMulticastDelegate::register_bindings()
  {
  tBindingBase::register_bindings("MulticastDelegate");

  ms_class_p->register_method_func_bulk(SkUEMulticastDelegate_Impl::methods_i, A_COUNT_OF(SkUEMulticastDelegate_Impl::methods_i), SkBindFlag_instance_no_rebind);
  }

//---------------------------------------------------------------------------------------

SkClass * SkUEMulticastDelegate::get_class()
  {
  return ms_class_p;
  }

//---------------------------------------------------------------------------------------

void SkUEMulticastDelegate::Instance::delete_this()
  {
  new (this) SkInstance(ALeaveMemoryUnchanged); // Reset v-table back to SkInstance
  SkInstance::delete_this(); // And recycle SkInstance
  }

//---------------------------------------------------------------------------------------

void SkUEMulticastDelegate::Instance::invoke_as_method(SkObjectBase * scope_p, SkInvokedBase * caller_p, SkInstance ** result_pp, const SkClosureInvokeInfo & invoke_info, const SkExpressionBase * invoking_expr_p) const
  {
  uint32_t invoked_data_array_size = invoke_info.m_params_p->get_arg_count_total();
  SkInvokedMethod imethod(caller_p, const_cast<Instance *>(this), invoked_data_array_size, a_stack_allocate(invoked_data_array_size, SkInstance*));

  // Store expression debug info for next invoked method/coroutine.
  SKDEBUG_ICALL_STORE_GEXPR(invoking_expr_p);

  // Must be called before calling argument expressions
  SKDEBUG_ICALL_SET_EXPR(&imethod, invoking_expr_p);

  // Append argument list
  imethod.data_append_args_exprs(invoke_info.m_arguments, *invoke_info.m_params_p, scope_p);

  // Hook must be called after argument expressions and before invoke()
  SKDEBUG_HOOK_EXPR(invoking_expr_p, scope_p, &imethod, nullptr, SkDebug::HookContext_peek);

  // Invoke the UE4 function
  SkUEReflectionManager::get()->invoke_k2_delegate(as<SkUEMulticastDelegate>(), invoke_info.m_params_p, &imethod, result_pp);

  /* $Revisit - MBreyer Enable this when we support return arguments
  // Bind any return arguments
  if (!invoke_info.m_return_args.is_empty())
    {
    imethod.data_bind_return_args(invoke_info.m_return_args, *invoke_info.m_params_p);
    }
  */
  }
