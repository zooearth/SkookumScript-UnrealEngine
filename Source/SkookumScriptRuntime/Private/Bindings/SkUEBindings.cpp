//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript Unreal Engine Bindings
// 
// Author: Markus Breyer
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include "../SkookumScriptRuntimePrivatePCH.h"

#include "SkUEBindings.hpp"

#include "VectorMath/SkVector2.hpp"
#include "VectorMath/SkVector3.hpp"
#include "VectorMath/SkVector4.hpp"
#include "VectorMath/SkRotation.hpp"
#include "VectorMath/SkRotationAngles.hpp"
#include "VectorMath/SkTransform.hpp"
#include "VectorMath/SkColor.hpp"

#include "Engine/SkUEName.hpp"
#include "Engine/SkUEActor.hpp"
#include "Engine/SkUEActorComponent.hpp"
#include "Engine/SkUEEntity.hpp"
#include "Engine/SkUEEntityClass.hpp"
#include "Engine/SkUESkookumScriptBehaviorComponent.hpp"

//=======================================================================================
// Engine-Generated
//=======================================================================================

// Massive code file containing thousands of generated bindings
#include <SkUEGeneratedBindings.generated.inl> 

static SkUEGeneratedBindings s_generated_bindings;

//=======================================================================================
// SkUEBindings Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Registers UE classes, structs and enums
void SkUEBindings::register_static_ue_types(SkUEBindingsInterface * game_generated_bindings_p)
  {
  // Register generated classes
  s_generated_bindings.register_static_ue_types();
  if (game_generated_bindings_p)
    {
    game_generated_bindings_p->register_static_ue_types();
    }

  // Manually register additional classes
  SkUESkookumScriptBehaviorComponent::ms_uclass_p = USkookumScriptBehaviorComponent::StaticClass();
  SkUEClassBindingHelper::register_static_class(SkUESkookumScriptBehaviorComponent::ms_uclass_p);
  #if WITH_EDITOR || HACK_HEADER_GENERATOR
    SkUESkookumScriptBehaviorComponent::ms_uclass_p->SetMetaData(TEXT("IsBlueprintBase"), TEXT("true")); // So it will get recognized as a component allowing Blueprints to be derived from
  #endif
  }

//---------------------------------------------------------------------------------------
// Registers bindings for SkookumScript
void SkUEBindings::register_all_bindings(SkUEBindingsInterface * game_generated_bindings_p)
  {
  // Core Overlay
  SkBoolean::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_boolean);
  SkInteger::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_integer);
  SkReal::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_struct<SkReal>);
  SkString::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_string);
  SkEnum::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_enum);
  SkList::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_list);

  // VectorMath Overlay
  SkVector2::register_bindings();
  SkVector3::register_bindings();
  SkVector4::register_bindings();
  SkRotation::register_bindings();
  SkRotationAngles::register_bindings();
  SkTransform::register_bindings();
  SkColor::register_bindings();

  // Engine-Generated/Project-Generated-C++ Overlay
  // Register static Sk types on both overlays, but register bindings only on one of them
  s_generated_bindings.register_static_sk_types();
  if (game_generated_bindings_p)
    {
    game_generated_bindings_p->register_static_sk_types();
    game_generated_bindings_p->register_bindings();
    }
  else
    {
    s_generated_bindings.register_bindings();
    }

  // Engine Overlay
  SkUEEntity_Ext::register_bindings();
  SkUEEntityClass_Ext::register_bindings();
  SkUEActor_Ext::register_bindings();
  SkUEActorComponent_Ext::register_bindings();
  SkUESkookumScriptBehaviorComponent::register_bindings();
  SkUEName::register_bindings();
  SkUEName::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_struct<SkUEName>);
  }
