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
#include "Engine/SkUEEntity.hpp"
#include "Engine/SkUEEntityClass.hpp"
#include "Engine/SkUESkookumScriptBehaviorComponent.hpp"

//=======================================================================================
// Engine-Generated
//=======================================================================================

// HACK to support UMG module
#include "MovieScene.h"
#include "UserWidget.h"
#include "Widgets/Layout/SScaleBox.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations" // Generated code will use some deprecated functions - that's ok don't tell me about it
#endif

#include <SkUE.generated.inl> // Massive code file containing thousands of generated bindings

#ifdef __clang__
#pragma clang diagnostic pop
#endif

//=======================================================================================
// Data
//=======================================================================================

// HACK - to fix linker error - remove this if it causes trouble
#if !IS_MONOLITHIC
  namespace FNavigationSystem
    {
    const float FallbackAgentRadius = 35.f;
    const float FallbackAgentHeight = 144.f;
    }
#endif

//=======================================================================================
// SkUEBindings Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Registers UE classes, structs and enums
void SkUEBindings::register_static_types()
  {
  // Register generated classes
  SkUE::register_static_types();

  // Manually register additional classes
  SkUESkookumScriptBehaviorComponent::ms_uclass_p = USkookumScriptBehaviorComponent::StaticClass();
  SkUESkookumScriptBehaviorComponent::ms_uclass_p->SetMetaData(TEXT("IsBlueprintBase"), TEXT("true")); // So it will get recognized as a component allowing Blueprints to be derived from
  SkUEClassBindingHelper::register_static_class(SkUESkookumScriptBehaviorComponent::ms_uclass_p);
  }

//---------------------------------------------------------------------------------------
// Registers bindings for SkookumScript
void SkUEBindings::register_all_bindings()
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

  // Engine-Generated Overlay
  SkUE::register_bindings();

  // Engine Overlay
  SkUEEntity_Ext::register_bindings();
  SkUEEntityClass_Ext::register_bindings();
  SkUEActor_Ext::register_bindings();
  SkUESkookumScriptBehaviorComponent::register_bindings();
  SkUEName::register_bindings();
  SkUEName::get_class()->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_struct<SkUEName>);
  }
