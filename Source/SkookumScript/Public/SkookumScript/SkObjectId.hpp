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

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkExpressionBase.hpp>


//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// SkookumScript Validated Object Identifier - used to identify/look-up an object
// instance based on its class and name and validated before runtime.
// 
// #Author(s) Conan Reis
class SK_API SkObjectIDBase : public SkExpressionBase
  {
  public:

  // Nested structures

    // Class flags - stored in m_flags
    // Currently serialized out as
    enum eFlag
      {
      Flag_possible    = 1 << 0,  // Class@?'name' Indeterminate find which may return nil or object and does not have error if nil
      Flag_identifier  = 1 << 1,  // Class@#'name' Identifier name result instead of object

      Flag__custom_bit      = 3,       // This bit and up may be used by custom look-up
      Flag__none            = 0,
      Flag__variant_mask = Flag_possible | Flag_identifier,  // Used with eVariant
      Flag__default         = Flag__none
      };

    // The variant/kind of an Object ID
    enum eVariant
      {
      Variant_reference    = Flag__none,     // Class@'name'
      Variant_possible_ref = Flag_possible,  // Class@?'name'
      Variant_identifier   = Flag_identifier // Class@#'name'
      };


  // Public Data Members

    // Class to retrieve object from
    SkClass * m_class_p;

    // Cached smart pointer to object
    mutable AIdPtr<SkInstance> m_obj_p;

    // Flags - see SkObjectIDBase::eFlag
    uint32_t m_flags;


  // Common Methods

    SK_NEW_OPERATORS(SkObjectIDBase);

    SkObjectIDBase() : m_class_p(nullptr), m_flags(Flag__default) {}
    SkObjectIDBase(SkClass * class_p, uint32_t flags);
    SkObjectIDBase(const SkObjectIDBase & source) : m_class_p(source.m_class_p), m_obj_p(source.m_obj_p), m_flags(source.m_flags) {}


  // Converter Methods

    #if (SKOOKUM & SK_COMPILED_IN)
      void assign_binary(const void ** binary_pp);
    #endif


    #if (SKOOKUM & SK_COMPILED_OUT)
      virtual void     as_binary(void ** binary_pp) const override;
      virtual uint32_t as_binary_length() const override;
    #endif


    #if (SKOOKUM & SK_CODE_IN)
      virtual const SkExpressionBase * find_expr_last_no_side_effect() const override  { return this; }
      virtual eSkSideEffect            get_side_effect() const override                { return SkSideEffect_none; }
      virtual SkClass *                validate(bool validate_deferred = true) = 0;
    #endif


    #if defined(SK_AS_STRINGS)
      virtual AString as_code() const override;
    #endif


  // Methods

    eVariant get_variant() const  { return eVariant(m_flags & Flag__variant_mask); }

    virtual AString         get_name() const = 0;
    virtual SkInstance *    get_name_instance() const = 0;
    virtual eSkExprType     get_type() const override;
    virtual SkInvokedBase * invoke(SkObjectBase * scope_p, SkInvokedBase * caller_p = nullptr, SkInstance ** result_pp = nullptr) const override;

  };  // SkObjectIDBase


//---------------------------------------------------------------------------------------
// ASymbol version of Validated Object ID
// - used by SkookumIDE and engines using ASymbol as main identifier type.
// 
// #Author(s) Conan Reis
class SK_API SkObjectIDSymbol : public SkObjectIDBase
  {
  public:
  // Public Data Members

    // Name of object
    ASymbol m_name;

  // Common Methods

    SK_NEW_OPERATORS(SkObjectIDSymbol);

    SkObjectIDSymbol() {}
    SkObjectIDSymbol(const ASymbol & name, SkClass * class_p, uint32_t flags);
    SkObjectIDSymbol(const SkObjectIDSymbol & source) : SkObjectIDBase(source), m_name(source.m_name) {}

  // Converter Methods

    #if (SKOOKUM & SK_COMPILED_IN)
      SkObjectIDSymbol(const void ** binary_pp);
    #endif

  // Methods

    virtual AString      get_name() const override           { return m_name.as_str_dbg(); }
    virtual SkInstance * get_name_instance() const override;
    virtual void            track_memory(AMemoryStats * mem_stats_p) const override;

    #if (SKOOKUM & SK_CODE_IN)
      virtual SkClass * validate(bool validate_deferred = true) override;
    #endif

  };  // SkObjectIDSymbol


//=======================================================================================
// Inline Methods
//=======================================================================================

#ifndef A_INL_IN_CPP
  #include <SkookumScript/SkObjectID.inl>
#endif

