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

#include "SkookumScriptClassDataComponent.h"
#include "Bindings/Engine/SkUEActor.hpp"

//=======================================================================================
// Class Data
//=======================================================================================

//=======================================================================================
// Method Definitions
//=======================================================================================

//---------------------------------------------------------------------------------------

USkookumScriptClassDataComponent::USkookumScriptClassDataComponent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  {
  PrimaryComponentTick.bCanEverTick = false;
  bTickInEditor = false;
  bAutoActivate = true;
  bWantsInitializeComponent = true;
  bWantsBeginPlay = true;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::create_sk_instance()
  {
  SK_ASSERTX(!m_actor_instance_p, "Tried to create actor instance when instance already present!");

  // Find the actor I belong to
  AActor * actor_p = GetOwner();
  SK_ASSERTX(actor_p, "SkookumScriptClassDataComponent must be attached to an actor.");

  // Determine SkookumScript class of my actor
  SkClass * class_p = nullptr;
  FString class_name = ScriptActorClassName;
  if (!class_name.IsEmpty())
    {
    AString class_name_ascii(*class_name, class_name.Len());
    class_p = SkBrain::get_class(class_name_ascii.as_cstr());
    SK_ASSERTX(class_p, a_cstr_format("Cannot find Script Class Name '%s' specified in SkookumScriptClassDataComponent of '%S'. Misspelled?", class_name_ascii.as_cstr(), *actor_p->GetName()));
    if (!class_p) goto set_default_class; // Recover from bad user input

    // Do some extra checking in non-shipping builds
    // Do some extra checking in non-shipping builds
    #if (SKOOKUM & SK_DEBUG)
      SkClass * super_class_known_to_ue_p = SkUEClassBindingHelper::find_most_derived_super_class_known_to_ue(class_p);
      SkClass * super_class_known_to_sk_p = SkUEClassBindingHelper::find_most_derived_super_class_known_to_sk(actor_p->GetClass());
      SK_ASSERTX(super_class_known_to_sk_p && super_class_known_to_sk_p == super_class_known_to_ue_p, a_cstr_format(
        "Owner Script Class Name '%s' in SkookumScriptClassDataComponent of '%S' is not properly related to Actor. "
        "Both the Actor Script Class Name '%s' and the UE4 class of '%S' ('%S') must share the topmost ancestor class known to both SkookumScript and UE4. "
        "Right now these ancestor classes are different ('%s' for '%s' and '%s' for '%S').",
        class_name_ascii.as_cstr(),
        *actor_p->GetName(),
        class_name_ascii.as_cstr(),
        *actor_p->GetName(),
        *actor_p->GetClass()->GetName(),
        super_class_known_to_ue_p ? super_class_known_to_ue_p->get_name_cstr_dbg() : "<none>",
        class_name_ascii.as_cstr(),
        super_class_known_to_sk_p ? super_class_known_to_sk_p->get_name_cstr_dbg() : "<none>",
        *actor_p->GetClass()->GetName()));
    #endif
    }
  else
    {
  set_default_class:
    class_p = SkUEClassBindingHelper::find_most_derived_super_class_known_to_sk(actor_p->GetClass());
    SK_ASSERTX(class_p, a_cstr_format("No parent class of %S is known to SkookumScript!", *actor_p->GetClass()->GetName()));
    if (!class_p)
      {
      class_p = SkBrain::ms_actor_class_p; // Recover to prevent crash
      }
    }

  // Based on the desired class, create SkInstance or SkDataInstance
  // Currently, we support only actors and minds
  SK_ASSERTX(class_p->is_actor_class(), a_str_format("Trying to create a SkookumScriptClassDataComponent of class '%s' which is not an actor.", class_p->get_name_cstr_dbg()));
  SkInstance * instance_p = class_p->new_instance();
  if (class_p->is_actor_class())
    {
    instance_p->construct<SkUEActor>(actor_p); // Keep track of owner actor
    }
  m_actor_instance_p = instance_p;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::delete_sk_instance()
  {
  SK_ASSERTX(m_actor_instance_p, "No Sk instance to delete!");
  m_actor_instance_p->clear_coroutines();
  m_actor_instance_p->dereference();
  m_actor_instance_p = nullptr;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::OnRegister()
  {
  Super::OnRegister();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::InitializeComponent()
  {
  Super::InitializeComponent();

  // Create SkookumScript instance, but only if we are located inside the game world
  if (GetOwner()->GetWorld()->IsGameWorld())
    {
    SK_ASSERTX(SkookumScript::get_initialization_level() >= SkookumScript::InitializationLevel_gameplay, "SkookumScript must be in gameplay mode when InitializeComponent() is invoked.");

    create_sk_instance();
    m_actor_instance_p->get_class()->resolve_raw_data();
    m_actor_instance_p->call_default_constructor();
    }
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::BeginPlay()
  {
  Super::BeginPlay();
  SK_ASSERTX(m_actor_instance_p != nullptr, a_str_format("SkookumScriptClassDataComponent '%S' on actor '%S' has no SkookumScript instance upon BeginPlay. This means its InitializeComponent() method was never called during initialization. Please check your initialization sequence and make sure this component gets properly initialized.", *GetName(), *GetOwner()->GetName()));
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::EndPlay(const EEndPlayReason::Type end_play_reason)
	{
  Super::EndPlay(end_play_reason);
	}

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::UninitializeComponent()
  {
  // Delete SkookumScript instance if present
  if (m_actor_instance_p)
    {
    SK_ASSERTX(SkookumScript::get_initialization_level() >= SkookumScript::InitializationLevel_gameplay, "SkookumScript must be in gameplay mode when UninitializeComponent() is invoked.");

    delete_sk_instance();
    }

  Super::UninitializeComponent();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptClassDataComponent::OnUnregister()
  {
  Super::OnUnregister();
  }

