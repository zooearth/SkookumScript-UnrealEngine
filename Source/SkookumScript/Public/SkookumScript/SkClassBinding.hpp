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
// Base class for binding mechanism class providing default 
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkClassBindingBase.hpp>

//---------------------------------------------------------------------------------------
// Class binding for simple types that can auto-construct
template<class _BindingClass, typename _DataType>
class SkClassBindingSimple : public SkClassBindingBase<_BindingClass, _DataType>
  {
  };

//---------------------------------------------------------------------------------------
// Class binding for zero-initialized types that have no destructor
template<class _BindingClass, typename _DataType>
class SkClassBindingSimpleZero : public SkClassBindingBase<_BindingClass, _DataType>
  {
  public:
    // No destructor
    enum { Binding_has_dtor = false };
    // Constructor initializes with zero
    static void mthd_ctor(SkInvokedMethod * scope_p, SkInstance ** result_pp) { scope_p->get_this()->construct<_BindingClass>(_DataType(0)); }
  };
