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
// Component to call SkookumScript ctor and dtor at the proper moment
//=======================================================================================

//=======================================================================================
// Includes
//=======================================================================================

#include "SkookumScriptRuntimePrivatePCH.h"

#include "SkookumScriptConstructionComponent.h"
#include "SkookumScriptClassDataComponent.h"
#include "SkookumScriptInstanceProperty.h"
#include "Bindings/Engine/SkUEActor.hpp"
#include "SkUEEntity.generated.hpp"

#include "Engine/World.h"
#include "Runtime/Launch/Resources/Version.h" // TEMP HACK for ENGINE_MINOR_VERSION

//=======================================================================================
// Class Data
//=======================================================================================

//=======================================================================================
// Method Definitions
//=======================================================================================

//---------------------------------------------------------------------------------------

USkookumScriptConstructionComponent::USkookumScriptConstructionComponent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  {
  PrimaryComponentTick.bCanEverTick = false;
  bTickInEditor = false;
  bAutoActivate = true;
  bWantsInitializeComponent = true;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptConstructionComponent::InitializeComponent()
  {
  Super::InitializeComponent();

  // Create SkookumScript instance, but only if we are located inside the game world
  // If there's a USkookumScriptClassDataComponent, it must be ahead in the queue since we checked before this component was attached
  // Therefore, if there's one, we'll leave the construction of the instance to it
  AActor * actor_p = GetOwner();
  if (actor_p->GetWorld()->IsGameWorld()
   && !actor_p->GetComponentByClass(USkookumScriptClassDataComponent::StaticClass()))
    {
    SK_ASSERTX(SkookumScript::get_initialization_level() >= SkookumScript::InitializationLevel_gameplay, "SkookumScript must be in gameplay mode when InitializeComponent() is invoked.");

    SkClass * sk_class_p = SkUEClassBindingHelper::get_sk_class_from_ue_class(actor_p->GetClass());
    if (sk_class_p)
      {
      uint32_t offset = sk_class_p->get_user_data_int();
      if (!offset)
        {
        // If offset has not been computed yet, compute it now
        UProperty * property_p = actor_p->GetClass()->FindPropertyByName(USkookumScriptInstanceProperty::StaticClass()->GetFName());
        SK_ASSERTX(property_p, a_str_format("Class '%s' has no USkookumScriptInstanceProperty needed for actor '%S'!", sk_class_p->get_name_cstr(), *actor_p->GetName()));
        if (property_p)
          {
          offset = property_p->GetOffset_ForInternal();
          sk_class_p->set_user_data_int_recursively(offset);
          }
        }
      SK_ASSERTX(offset, a_str_format("Class '%s' has no embedded instance offset to create an SkInstance for actor '%S'!", sk_class_p->get_name_cstr(), *actor_p->GetName()));
      if (offset)
        {
        USkookumScriptInstanceProperty::construct_instance((uint8_t *)actor_p + offset, actor_p, sk_class_p);
        }
      }
    }
  }

//---------------------------------------------------------------------------------------

void USkookumScriptConstructionComponent::UninitializeComponent()
  {
  // Delete SkookumScript instance if present
  SK_ASSERTX(SkookumScript::get_initialization_level() >= SkookumScript::InitializationLevel_sim, "SkookumScript must be at least in sim mode when UninitializeComponent() is invoked.");

  AActor * actor_p = GetOwner();
  // Only uninitialize those components that we initialized to begin with
  // If there's a USkookumScriptClassDataComponent, it has already taken care of the destruction
  if (actor_p->GetWorld() && actor_p->GetWorld()->IsGameWorld()
   && !actor_p->GetComponentByClass(USkookumScriptClassDataComponent::StaticClass()))
    {
    SkClass * sk_class_p = SkUEClassBindingHelper::get_sk_class_from_ue_class(actor_p->GetClass());
    if (sk_class_p)
      {
      uint32_t offset = sk_class_p->get_user_data_int();
      SK_ASSERTX(offset, a_str_format("Class '%s' has no embedded instance offset to destroy the SkInstance of actor '%S'!", sk_class_p->get_name_cstr(), *actor_p->GetName()));
      if (offset)
        {
        USkookumScriptInstanceProperty::destroy_instance((uint8_t *)actor_p + offset);
        }
      }
    }

  Super::UninitializeComponent();
  }
