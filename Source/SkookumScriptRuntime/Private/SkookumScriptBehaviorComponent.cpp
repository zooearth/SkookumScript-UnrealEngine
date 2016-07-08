//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2016 Agog Labs Inc. All rights reserved.
//
// Component to associate a SkookumScript class and data members with a UE4 actor
// and allow SkookumScript ctors and dtors to be called when the actor (i.e. the component) gets created/destroyed
// 
// Author: Conan Reis, Markus Breyer
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include "SkookumScriptRuntimePrivatePCH.h"
#include "../Classes/SkookumScriptBehaviorComponent.h"

#include "VectorField/VectorField.h" // HACK to fix broken dependency on UVectorField 
#include <SkUEEnums.generated.hpp>

#include "Bindings/Engine/SkUESkookumScriptBehaviorComponent.hpp"


//=======================================================================================
// Class Data
//=======================================================================================

ASymbol USkookumScriptBehaviorComponent::ms_symbol_on_attach;
ASymbol USkookumScriptBehaviorComponent::ms_symbol_on_detach;
ASymbol USkookumScriptBehaviorComponent::ms_symbol_on_begin_play;
ASymbol USkookumScriptBehaviorComponent::ms_symbol_on_end_play;

//=======================================================================================
// Method Definitions
//=======================================================================================

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::initialize()
  {
  ms_symbol_on_attach     = ASymbol::create_existing("on_attach");
  ms_symbol_on_detach     = ASymbol::create_existing("on_detach");
  ms_symbol_on_begin_play = ASymbol::create_existing("on_begin_play");
  ms_symbol_on_end_play   = ASymbol::create_existing("on_end_play");
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::deinitialize()
  {
  ms_symbol_on_attach     = ASymbol::get_null();
  ms_symbol_on_detach     = ASymbol::get_null();
  ms_symbol_on_begin_play = ASymbol::get_null();
  ms_symbol_on_end_play   = ASymbol::get_null();
  }

//---------------------------------------------------------------------------------------

USkookumScriptBehaviorComponent::USkookumScriptBehaviorComponent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_is_instance_externally_owned(false)
  {
  PrimaryComponentTick.bCanEverTick = false;
  bTickInEditor = false;
  bAutoActivate = true;
  bWantsInitializeComponent = true;
  bWantsBeginPlay = true;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::create_sk_instance()
  {
  SK_ASSERTX(!m_component_instance_p, "Tried to create actor instance when instance already present!");

  // Find the actor I belong to
  AActor * actor_p = GetOwner();
  SK_ASSERTX(actor_p, "SkookumScriptBehaviorComponent must be attached to an actor.");

  FString class_name = ScriptComponentClassName;
  AString class_name_ascii(*class_name, class_name.Len());
  SkClass * class_p = SkBrain::get_class(class_name_ascii.as_cstr());
  SK_ASSERTX(class_p, a_cstr_format("Cannot find Script Class Name '%s' specified in SkookumScriptBehaviorComponent of '%S'. Misspelled?", class_name_ascii.as_cstr(), *actor_p->GetName()));
  if (!class_p)
    {
    class_p = SkBrain::get_class(SkBrain::ms_component_class_name); // Recover from bad user input
    }

  // Based on the desired class, create SkInstance or SkDataInstance
  // Must be derived from SkookumScriptBehaviorComponent
  SK_ASSERTX(class_p->is_component_class(), a_str_format("Trying to create a SkookumScriptBehaviorComponent of class '%s' which is not derived from SkookumScriptBehaviorComponent.", class_p->get_name_cstr_dbg()));
  m_component_instance_p = class_p->new_instance();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::delete_sk_instance()
  {
  SK_ASSERTX(m_component_instance_p, "No Sk instance to delete!");
  m_component_instance_p->clear_coroutines();
  m_component_instance_p->dereference();
  m_component_instance_p = nullptr;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::set_sk_component_instance(SkInstance * component_instance_p)
  {
  SK_ASSERTX(!m_component_instance_p, "Tried to create actor instance when instance already present!");

  component_instance_p->reference();
  m_component_instance_p = component_instance_p;
  m_is_instance_externally_owned = true;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::OnRegister()
  {
  Super::OnRegister();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::InitializeComponent()
  {
  Super::InitializeComponent();

  // Create SkookumScript instance, but only if we are located inside the game world
  if (GetOwner()->GetWorld() == SkUEClassBindingHelper::get_world())
    {
    if (!m_is_instance_externally_owned)
      {
      SK_ASSERTX(SkookumScript::is_flag_set(SkookumScript::Flag_evaluate), "SkookumScript must be in initialized state when InitializeComponent() is invoked.");
      create_sk_instance();
      m_component_instance_p->get_class()->resolve_raw_data();
      m_component_instance_p->call_default_constructor();
      }

    m_component_instance_p->as<SkUESkookumScriptBehaviorComponent>() = this;
    m_component_instance_p->method_call(ms_symbol_on_attach);
    }
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::BeginPlay()
  {
  Super::BeginPlay();

  m_component_instance_p->method_call(ms_symbol_on_begin_play);
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::EndPlay(const EEndPlayReason::Type end_play_reason)
  {
  m_component_instance_p->method_call(ms_symbol_on_end_play, SkUEEEndPlayReason::new_instance(end_play_reason));

  Super::EndPlay(end_play_reason);
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::UninitializeComponent()
  {
  // Delete SkookumScript instance if present
  if (m_component_instance_p)
    {
    SK_ASSERTX(SkookumScript::is_flag_set(SkookumScript::Flag_evaluate), "SkookumScript must be in initialized state when UninitializeComponent() is invoked.");

    m_component_instance_p->method_call(ms_symbol_on_detach);
    m_component_instance_p->as<SkUESkookumScriptBehaviorComponent>() = nullptr;

    if (m_is_instance_externally_owned)
      {
      m_component_instance_p->dereference();
      m_component_instance_p = nullptr;
      m_is_instance_externally_owned = false;
      }
    else
      {
      delete_sk_instance();
      }
    }

  Super::UninitializeComponent();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptBehaviorComponent::OnUnregister()
  {
  Super::OnUnregister();
  }

