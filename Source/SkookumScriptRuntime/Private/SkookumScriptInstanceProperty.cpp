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


//=======================================================================================
// Includes
//=======================================================================================

#include "SkookumScriptRuntimePrivatePCH.h"

#include "SkookumScriptInstanceProperty.h"
#include "SkookumScriptClassDataComponent.h"
#include "SkookumScriptConstructionComponent.h"
#include "Bindings/SkUEClassBinding.hpp"
#include "SkUEEntity.generated.hpp"

#include <SkookumScript/SkInstance.hpp>

#include "Engine/World.h"
#include "UObject/PropertyPortFlags.h"

//=======================================================================================
// Method Definitions
//=======================================================================================

//---------------------------------------------------------------------------------------

USkookumScriptInstanceProperty::USkookumScriptInstanceProperty(const FObjectInitializer & object_initializer)
  : Super(object_initializer)
  {
  ArrayDim = 1;
  #if WITH_EDITORONLY_DATA
    ElementSize = sizeof(AIdPtr<SkInstance>);
  #else
    ElementSize = sizeof(SkInstance *);
  #endif
  }

//---------------------------------------------------------------------------------------

USkookumScriptInstanceProperty::~USkookumScriptInstanceProperty()
  {
  }

//---------------------------------------------------------------------------------------
// Create an SkInstance and call its default constructor
SkInstance * USkookumScriptInstanceProperty::construct_instance(void * data_p, UObject * obj_p, SkClass * sk_class_p)
  {
  SkInstance * instance_p = sk_class_p->new_instance(); // SkInstance or SkDataInstance
  instance_p->construct<SkUEEntity>(obj_p);             // SkInstance points back to the owner object
  set_instance(data_p, instance_p);                     // Store SkInstance in object
  instance_p->call_default_constructor();               // Call script constructor    
  return instance_p;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::destroy_instance(void * data_p)
  {
  SkInstance * instance_p = get_instance(data_p); // Get SkInstance from object
  if (instance_p)
    {
    instance_p->abort_coroutines_on_this();
    // Destructor not explicitly called here as it will be automagically called 
    // when the instance is dereferenced to zero
    instance_p->dereference();
    // Zero the pointer so it's fresh in case UE4 wants to recycle this object
    set_instance(data_p, nullptr);
    }
  }

//---------------------------------------------------------------------------------------

FORCEINLINE UObject * USkookumScriptInstanceProperty::get_owner(const void * data_p) const
  {
  return (UObject *)((uint8_t *)data_p - GetOffset_ForInternal());
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::LinkInternal(FArchive & ar)
  {
  // Nothing to do here, but function must exist
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::Serialize(FArchive & ar)
  {
  // For now, we're not storing any additional data when we are serialized
  Super::Serialize(ar);
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::SerializeItem(FArchive & ar, void * data_p, void const * default_data_p) const
  {
  // This property merely reserves storage but doesn't store any actual data
  // So there's no need to serialize anything here
  }

//---------------------------------------------------------------------------------------

FString USkookumScriptInstanceProperty::GetCPPType(FString * extended_type_text_p, uint32 cpp_export_flags) const
  {
  // This property reserves storage - return dummy place holders
  #if WITH_EDITORONLY_DATA
    return TEXT("struct { void * m_p; uint32 id; }");
  #else
    return TEXT("void *");
  #endif
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::ExportTextItem(FString & value_str, const void * data_p, const void * default_data_p, UObject * owner_p, int32 port_flags, UObject * export_root_scope_p) const
  {
  // This property merely reserves storage but doesn't store any actual data
  // So return nullptr just to return something
  value_str += (port_flags & PPF_ExportCpp) ? TEXT("nullptr") : TEXT("NULL");
  }

//---------------------------------------------------------------------------------------

const TCHAR * USkookumScriptInstanceProperty::ImportText_Internal(const TCHAR * buffer_p, void * data_p, int32 port_flags, UObject * owner_p, FOutputDevice * error_text_p) const
  {
  // Consume the identifier that we stored ("NULL")
  FString temp; 
  buffer_p = UPropertyHelpers::ReadToken(buffer_p, temp);

  // Initialize value
  InitializeValueInternal(data_p);

  return buffer_p;
  }

//---------------------------------------------------------------------------------------

int32 USkookumScriptInstanceProperty::GetMinAlignment() const
  {
  return ALIGNOF(uintptr_t);
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::InitializeValueInternal(void * data_p) const
  {
  UObject * owner_p = get_owner(data_p);
  // Leave untouched on CDOs
  if (!(owner_p->GetFlags() & RF_ClassDefaultObject))
    {
    // Clear SkInstance storage in object
    set_instance(data_p, nullptr);

    AActor * actor_p = Cast<AActor>(owner_p);
    if (actor_p)
      {
      // If an actor, append a component to it that will take care of the proper construction at the proper time
      // i.e. after all other components are initialized
      // But only if there's not a SkookumScriptClassDataComponent already
      if (!actor_p->GetComponentByClass(USkookumScriptClassDataComponent::StaticClass()))
        {
        FName component_name = USkookumScriptConstructionComponent::StaticClass()->GetFName();
        USkookumScriptConstructionComponent * component_p = NewObject<USkookumScriptConstructionComponent>(actor_p, component_name);
        actor_p->AddOwnedComponent(component_p);
        //component_p->RegisterComponent();
        }
      }
    else
      {
      // Objects loaded by the editor are just loaded into the editor world, not the game world
      #if WITH_EDITOR
        if (GIsEditorLoadingPackage) return;
      #endif

      // Construct right here
      SkClass * sk_class_p = SkUEClassBindingHelper::get_sk_class_from_ue_class(owner_p->GetClass());
      //sk_class_p->resolve_raw_data(); // In case it wasn't resolved before
      USkookumScriptInstanceProperty::construct_instance(data_p, owner_p, sk_class_p);
      }
    }
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::InstanceSubobjects(void * data_p, void const * default_data_p, UObject * owner_p, struct FObjectInstancingGraph * instance_graph_p)
  {
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::ClearValueInternal(void * data_p) const
  {
  // This property merely reserves storage but doesn't store any actual data
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::DestroyValueInternal(void * data_p) const
  {
  UObject * owner_p = get_owner(data_p);
  // Leave untouched on CDOs
  if (!(owner_p->GetFlags() & RF_ClassDefaultObject))
    {
    // Leave actors alone as the component we attached will take care of itself
    AActor * actor_p = Cast<AActor>(owner_p);
    if (!actor_p)
      {
      // Not an actor, so take care of destruction here
      destroy_instance(data_p);
      }
    }
  }

//---------------------------------------------------------------------------------------

void USkookumScriptInstanceProperty::CopyValuesInternal(void * dst_p, void const * src_p, int32 count) const
  {
  // Copying instances between objects makes no sense, so we simply don't do anything here
  }

//---------------------------------------------------------------------------------------

bool USkookumScriptInstanceProperty::SameType(const UProperty * other_p) const
  {
  return Super::SameType(other_p);
  }

//---------------------------------------------------------------------------------------

bool USkookumScriptInstanceProperty::Identical(const void * ldata_p, const void * rdata_p, uint32 port_flags) const
  {
  // By definition, no object should use the same SkInstance as another object
  return false;
  }

