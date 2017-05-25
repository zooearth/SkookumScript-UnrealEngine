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
// Class for managing functions exposed to Blueprint graphs 
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "../SkookumScriptRuntimePrivatePCH.h"

#include "UObject/WeakObjectPtr.h"
#include "UObject/Class.h"

#include <AgogCore/AFunctionArg.hpp>
#include <AgogCore/APArray.hpp>
#include <AgogCore/AVArray.hpp>
#include <AgogCore/ASymbol.hpp>
#include <SkookumScript/SkBrain.hpp>
#include <SkookumScript/SkClass.hpp>
#include <SkookumScript/SkInvokableBase.hpp>
#include <SkookumScript/SkMethod.hpp>
#include <SkookumScript/SkParameters.hpp>

class SkClass;
class SkClassDescBase;
class SkInstance;
class SkInvokedMethod;
struct FFrame;

typedef AFunctionArgBase<UClass *>            tSkUEOnClassUpdatedFunc;
typedef AFunctionArgBase2<UFunction *, bool>  tSkUEOnFunctionUpdatedFunc;
typedef AFunctionArgBase<UClass *>            tSkUEOnFunctionRemovedFromClassFunc;

//---------------------------------------------------------------------------------------
// Class for managing functions exposed to Blueprint graphs
class SkUEReflectionManager
  {
  public:
                 SkUEReflectionManager();
                 ~SkUEReflectionManager();

    static SkUEReflectionManager * get() { return ms_singleton_p; }

    void         clear(tSkUEOnFunctionRemovedFromClassFunc * on_function_removed_from_class_f);
    bool         sync_all_from_sk(tSkUEOnFunctionRemovedFromClassFunc * on_function_removed_from_class_f);
    bool         sync_class_from_sk(SkClass * sk_class_p, tSkUEOnFunctionRemovedFromClassFunc * on_function_removed_from_class_f);
    bool         sync_all_to_ue(tSkUEOnFunctionUpdatedFunc * on_function_updated_f, bool is_final);

    static bool  does_class_need_instance_property(SkClass * sk_class_p);
    static bool  add_instance_property_to_class(UClass * ue_class_p, SkClass * sk_class_p);

    static bool  can_ue_property_be_reflected(UProperty * ue_property_p);

    static bool  is_skookum_reflected_call(UFunction * function_p);
    static bool  is_skookum_reflected_event(UFunction * function_p);

  protected:

    // We place this magic number in the rep offset to be able to tell if a UFunction is an Sk event
    // Potential false positive is ok since we are using it only to select which graph nodes to update
    enum { EventMagicRepOffset = 0xBEEF };

    // To keep track of a property/parameter's name, size and type
    struct TypedName : ANamed
      {
      ASymbol     m_sk_class_name;
      SkClass *   m_sk_class_p;
      uint32_t    m_byte_size;

      TypedName(const ASymbol & name, SkClass * sk_class_p) : ANamed(name), m_sk_class_name(sk_class_p->get_name()), m_sk_class_p(sk_class_p), m_byte_size(0) {}
      void rebind_to_sk() { m_sk_class_p = SkBrain::get_class(m_sk_class_name); }
      };

    typedef SkInstance *  (*tK2ParamFetcher)(FFrame & stack, const TypedName & typed_name);
    typedef SkInstance *  (*tK2ValueFetcher)(const void * value_p, const TypedName & typed_name);
    typedef void          (*tK2ValueAssigner)(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    typedef uint32_t      (*tSkValueStorer)(void * dest_p, SkInstance * value_p, const TypedName & typed_name);

    enum eReflectedFunctionType : uint8_t
      {
      ReflectedFunctionType_Call,   // Call from Blueprints into Sk
      ReflectedFunctionType_Event,  // Call from Sk into Blueprints
      };

    // Keep track of a binding between Blueprints and Sk
    struct ReflectedFunction : ANamed
      {
      SkInvokableBase *         m_sk_invokable_p;
      TWeakObjectPtr<UFunction> m_ue_function_p;
      TypedName                 m_result_type;
      uint16_t                  m_num_params;
      bool                      m_is_class_member;    // Copy of m_sk_invokable_p->is_class_member() in case m_sk_invokable_p goes bad
      eReflectedFunctionType    m_type;
      bool                      m_marked_for_delete_class;
      bool                      m_marked_for_delete_all;

      ReflectedFunction(SkInvokableBase * sk_invokable_p, uint32_t num_params, SkClass * result_sk_class_p, eReflectedFunctionType type)
        : ANamed(sk_invokable_p->get_name())
        , m_sk_invokable_p(sk_invokable_p)
        , m_ue_function_p(nullptr)
        , m_result_type(ASymbol::ms_null, result_sk_class_p)
        , m_num_params(num_params)
        , m_is_class_member(sk_invokable_p->is_class_member())
        , m_marked_for_delete_class(false)
        , m_marked_for_delete_all(false)
        , m_type(type)
        {}

      //void rebind_sk_invokable();

      };

    typedef APArrayFree<ReflectedFunction, ASymbol> tReflectedFunctions;

    // Parameter being passed into Sk from Blueprints
    struct ReflectedCallParam : TypedName
      {
      tK2ParamFetcher m_fetcher_p;

      ReflectedCallParam(const ASymbol & name, SkClass * sk_class_p) : TypedName(name, sk_class_p), m_fetcher_p(nullptr) {}
      };

    // Function binding (call from Blueprints into Sk)
    struct ReflectedCall : public ReflectedFunction
      {
      tSkValueStorer  m_result_getter;

      ReflectedCall(SkInvokableBase * sk_invokable_p, uint32_t num_params, SkClass * result_sk_class_p)
        : ReflectedFunction(sk_invokable_p, num_params, result_sk_class_p, ReflectedFunctionType_Call)
        , m_result_getter(nullptr)
        {}

      // The parameter entries are stored behind this structure in memory
      ReflectedCallParam *       get_param_array()       { return (ReflectedCallParam *)(this + 1); }
      const ReflectedCallParam * get_param_array() const { return (const ReflectedCallParam *)(this + 1); }
      };

    // Parameter being passed into Blueprints into Sk
    struct ReflectedEventParam : TypedName
      {
      tSkValueStorer    m_storer_p;
      tK2ValueAssigner  m_assigner_p;
      uint32_t          m_offset;

      ReflectedEventParam(const ASymbol & name, SkClass * sk_class_p) 
        : TypedName(name, sk_class_p)
        , m_storer_p(nullptr)
        , m_assigner_p(nullptr)
        , m_offset(0) {}
      };

    // Event binding (call from Sk into Blueprints)
    struct ReflectedEvent : public ReflectedFunction
      {
      tK2ValueFetcher  m_result_getter;

      mutable TWeakObjectPtr<UFunction> m_ue_function_to_invoke_p; // The copy of our method we actually can invoke

      ReflectedEvent(SkMethodBase * sk_method_p, uint32_t num_params, SkClass * result_sk_class_p)
        : ReflectedFunction(sk_method_p, num_params, result_sk_class_p, ReflectedFunctionType_Event)
        , m_result_getter(nullptr)
        {}

      // The parameter entries are stored behind this structure in memory
      ReflectedEventParam *       get_param_array()       { return (ReflectedEventParam *)(this + 1); }
      const ReflectedEventParam * get_param_array() const { return (const ReflectedEventParam *)(this + 1); }
      };

    // An Sk parameter or raw instance data member reflected to UE4 as UProperty
    struct ReflectedProperty : ANamed
      {
      UProperty *      m_ue_property_p;
      tK2ParamFetcher  m_k2_param_fetcher_p;
      tK2ValueFetcher  m_k2_value_fetcher_p;
      tK2ValueAssigner m_k2_value_assigner_p;
      tSkValueStorer   m_sk_value_storer_p;

      ReflectedProperty()
        : m_ue_property_p(nullptr)
        , m_k2_param_fetcher_p(nullptr)
        , m_k2_value_fetcher_p(nullptr)
        , m_k2_value_assigner_p(nullptr)
        , m_sk_value_storer_p(nullptr) 
        {}
      };

    typedef APSortedLogicalFree<ReflectedProperty, ASymbol> tReflectedProperties;

    // AVArray can only be declared with a struct/class as template argument
    struct FunctionIndex
      {
      uint16_t  m_idx;

      FunctionIndex(uint32_t idx) : m_idx((uint16_t)idx) {}
      };

    // An Sk class reflected to UE4
    struct ReflectedClass : ANamed
      {
      TWeakObjectPtr<UClass>  m_ue_static_class_p;  // UE4 equivalent of the class (might be a parent class if the actual class is Blueprint generated)
      AVArray<FunctionIndex>  m_functions;          // Indices of reflected member routines
      tReflectedProperties    m_properties;         // Reflected (raw) data members
      bool                    m_store_sk_instance;  // This class must have a USkookumScriptInstanceProperty attached to it

      ReflectedClass(ASymbol name) : ANamed(name), m_store_sk_instance(false) {}
      };

    typedef APSortedLogicalFree<ReflectedClass, ASymbol> tReflectedClasses;

    void                exec_method(FFrame & stack, void * const result_p, SkClass * class_scope_p, SkInstance * this_p);
    void                exec_class_method(FFrame & stack, void * const result_p);
    void                exec_instance_method(FFrame & stack, void * const result_p);
    void                exec_coroutine(FFrame & stack, void * const result_p);

    static void         mthd_trigger_event(SkInvokedMethod * scope_p, SkInstance ** result_pp);

    bool                sync_class_from_sk_recursively(SkClass * sk_class_p, tSkUEOnFunctionRemovedFromClassFunc * on_function_removed_from_class_f);
    bool                try_add_reflected_function(SkInvokableBase * sk_invokable_p);
    bool                try_update_reflected_function(SkInvokableBase * sk_invokable_p, ReflectedClass ** out_reflected_class_pp, int32_t * out_function_index_p);
    bool                add_reflected_call(SkInvokableBase * sk_invokable_p);
    bool                add_reflected_event(SkMethodBase * sk_method_p);
    bool                expose_reflected_function(uint32_t i, tSkUEOnFunctionUpdatedFunc * on_function_updated_f, bool is_final);
    int32_t             store_reflected_function(ReflectedFunction * reflected_function_p, ReflectedClass * reflected_class_p, int32_t function_index_to_use);
    void                delete_reflected_function(uint32_t function_index);
    static UFunction *  find_ue_function(SkInvokableBase * sk_invokable_p);
    static UFunction *  reflect_ue_function(SkInvokableBase * sk_invokable_p, ReflectedProperty * out_param_info_array_p, bool is_final);
    static bool         reflect_ue_property(UProperty * ue_property_p, ReflectedProperty * out_info_p = nullptr);
    UFunction *         build_ue_function(UClass * ue_class_p, SkInvokableBase * sk_invokable_p, eReflectedFunctionType binding_type, uint32_t binding_index, ReflectedProperty * out_param_info_array_p, bool is_final);
    UProperty *         build_ue_param(const ASymbol & sk_name, SkClassDescBase * sk_type_p, UFunction * ue_function_p, ReflectedProperty * out_info_p, bool is_final);
    UProperty *         build_ue_property(const ASymbol & sk_name, SkClassDescBase * sk_type_p, UObject * ue_outer_p, ReflectedProperty * out_info_p, bool is_final);
    static void         bind_event_method(SkMethodBase * sk_method_p);
    void                on_unknown_type(const ASymbol & sk_name, SkClassDescBase * sk_type_p, UObject * ue_outer_p);
    
    template<class _TypedName>
    static bool         have_identical_signatures(const tSkParamList & param_list, const _TypedName * param_array_p);

    template<class _TypedName>
    static void         rebind_params_to_sk(const tSkParamList & param_list, _TypedName * param_array_p);

    static SkInstance * fetch_k2_param_boolean(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_integer(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_real(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_string(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_vector2(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_vector3(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_vector4(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_rotation_angles(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_transform(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_struct_val(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_struct_ref(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_entity(FFrame & stack, const TypedName & typed_name);
    static SkInstance * fetch_k2_param_enum(FFrame & stack, const TypedName & typed_name);

    static SkInstance * fetch_k2_value_boolean(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_integer(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_real(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_string(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_vector2(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_vector3(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_vector4(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_rotation_angles(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_transform(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_struct_val(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_struct_ref(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_entity(const void * value_p, const TypedName & typed_name);
    static SkInstance * fetch_k2_value_enum(const void * value_p, const TypedName & typed_name);

    static void         assign_k2_value_boolean(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_integer(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_real(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_string(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_vector2(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_vector3(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_vector4(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_rotation_angles(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_transform(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_struct_val(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_struct_ref(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_entity(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);
    static void         assign_k2_value_enum(SkInstance * dest_p, const void * value_p, const TypedName & typed_name);

    static uint32_t     store_sk_value_boolean(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_integer(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_real(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_string(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_vector2(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_vector3(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_vector4(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_rotation_angles(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_transform(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_struct_val(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_struct_ref(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_entity(void * dest_p, SkInstance * value_p, const TypedName & typed_name);
    static uint32_t     store_sk_value_enum(void * dest_p, SkInstance * value_p, const TypedName & typed_name);

    tReflectedFunctions   m_reflected_functions;
    tReflectedClasses     m_reflected_classes;

    ASymbol               m_result_name;

    UPackage *            m_module_package_p;

    static SkUEReflectionManager * ms_singleton_p; // Hack, make it easy to access for callbacks
        
    static UScriptStruct *  ms_struct_vector2_p;
    static UScriptStruct *  ms_struct_vector3_p;
    static UScriptStruct *  ms_struct_vector4_p;
    static UScriptStruct *  ms_struct_rotation_angles_p;
    static UScriptStruct *  ms_struct_transform_p;

  };