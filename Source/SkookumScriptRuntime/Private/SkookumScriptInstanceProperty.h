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
// Property representing a SkookumScript SkInstance/SkDataInstance pointer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "UObject/UnrealType.h"

#include <AgogCore/AIdPtr.hpp>

#include "SkookumScriptInstanceProperty.generated.h"

//=======================================================================================
// Global Defines / Macros
//=======================================================================================

class SkInstance;
class SkClass;

//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// Property representing a SkookumScript SkInstance/SkDataInstance pointer
UCLASS()
class USkookumScriptInstanceProperty : public UProperty
  {
  
    GENERATED_UCLASS_BODY()

  public:

    static SkInstance * get_instance(const void * data_p);
    static void         set_instance(void * data_p, SkInstance * instance_p);
    static SkInstance * construct_instance(void * data_p, UObject * obj_p, SkClass * sk_class_p);
    static void         destroy_instance(void * data_p);

  protected:

    virtual ~USkookumScriptInstanceProperty() override;

    virtual void          LinkInternal(FArchive & ar) override;
    virtual void          Serialize(FArchive & ar) override;
    virtual void          SerializeItem(FArchive & ar, void * data_p, void const * default_data_p) const override;
    virtual FString       GetCPPType(FString * extended_type_text_p, uint32 cpp_export_flags) const override;
    virtual void          ExportTextItem(FString & value_str, const void * data_p, const void * default_data_p, UObject * owner_p, int32 port_flags, UObject * export_root_scope_p) const override;
    virtual const TCHAR * ImportText_Internal(const TCHAR * buffer_p, void * data_p, int32 port_flags, UObject * owner_p, FOutputDevice * error_text_p) const override;
    virtual int32         GetMinAlignment() const override;
    virtual void          InitializeValueInternal(void * data_p) const override;
    virtual void          InstanceSubobjects(void * data_p, void const * default_data_p, UObject * owner_p, struct FObjectInstancingGraph * instance_graph_p) override;
    virtual void          ClearValueInternal(void * data_p) const override;
    virtual void          DestroyValueInternal(void * data_p) const override;
    virtual void          CopyValuesInternal(void * dst_p, void const * src_p, int32 count) const override;
    virtual bool          SameType(const UProperty * other_p) const override;
    virtual bool          Identical(const void * ldata_p, const void * rdata_p, uint32 port_flags) const override;

    UObject *             get_owner(const void * data_p) const;

  };

//=======================================================================================
// Inline methods
//=======================================================================================

//---------------------------------------------------------------------------------------

FORCEINLINE SkInstance * USkookumScriptInstanceProperty::get_instance(const void * data_p)
  {
  #if WITH_EDITORONLY_DATA
    return *(AIdPtr<SkInstance> *)data_p;
  #else
    return *(SkInstance **)data_p;
  #endif
  }

//---------------------------------------------------------------------------------------

FORCEINLINE void USkookumScriptInstanceProperty::set_instance(void * data_p, SkInstance * instance_p)
  {
  #if WITH_EDITORONLY_DATA
    new (data_p) AIdPtr<SkInstance>(instance_p);
  #else
    *(SkInstance **)data_p = instance_p;
  #endif
  }

