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
// Component to associate a SkookumScript Mind with a UE4 actor
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include "SkookumScriptRuntimePrivatePCH.h"

#include "SkookumScriptMindComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Launch/Resources/Version.h" // TEMP HACK for ENGINE_MINOR_VERSION

#include <AgogCore/AString.hpp>
#include <SkookumScript/SkBrain.hpp>
#include <SkookumScript/SkClass.hpp>
#include <SkookumScript/SkDebug.hpp>
#include <SkookumScript/SkMind.hpp>

//=======================================================================================
// Class Data
//=======================================================================================

//=======================================================================================
// Method Definitions
//=======================================================================================

//---------------------------------------------------------------------------------------

USkookumScriptMindComponent::USkookumScriptMindComponent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  {
  PrimaryComponentTick.bCanEverTick = false;
  bTickInEditor = false;
  bAutoActivate = true;
  bWantsInitializeComponent = true;
  #if ENGINE_MINOR_VERSION < 14
    bWantsBeginPlay = true;
  #endif
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::create_sk_instance()
  {
  SK_ASSERTX(!m_mind_instance_p, "Tried to create mind instance when instance already present!");

  // Find the actor I belong to
  AActor * actor_p = GetOwner();
  SK_ASSERTX(actor_p, "SkookumScriptMindComponent must be attached to an actor.");

  // Determine SkookumScript class of my Mind
  SkClass * class_p = nullptr;
  FString class_name = ScriptMindClassName;
  AString class_name_ascii(*class_name, class_name.Len());
  class_p = SkBrain::get_class(class_name_ascii.as_cstr());
  SK_ASSERTX(class_p, a_cstr_format("Cannot find Script Class Name '%s' specified in SkookumScriptMindComponent of '%S'. Misspelled?", class_name_ascii.as_cstr(), *actor_p->GetName()));
  if (!class_p)
    {
    class_p = SkBrain::ms_master_class_p; // Recover from bad user input
    }

  // Based on the desired class, create SkInstance or SkDataInstance
  SK_ASSERTX(class_p->is_mind_class(), a_str_format("Trying to create a SkookumScriptMindComponent of class '%s' which is not a Mind.", class_p->get_name_cstr_dbg()));
  m_mind_instance_p = class_p->new_instance();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::delete_sk_instance()
  {
  SK_ASSERTX(m_mind_instance_p, "No Sk instance to delete!");
  static_cast<SkMind *>(m_mind_instance_p.get_obj())->abort_coroutines();
  m_mind_instance_p->abort_coroutines_on_this();
  m_mind_instance_p->dereference();
  m_mind_instance_p = nullptr;
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::OnRegister()
  {
  Super::OnRegister();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::InitializeComponent()
  {
  Super::InitializeComponent();

  // Create SkookumScript instance, but only if we are located inside the game world
  if (GetOwner()->GetWorld()->IsGameWorld())
    {
    SK_ASSERTX(SkookumScript::get_initialization_level() >= SkookumScript::InitializationLevel_gameplay, "SkookumScript must be in gameplay mode when InitializeComponent() is invoked.");

    create_sk_instance();
    m_mind_instance_p->get_class()->resolve_raw_data();
    m_mind_instance_p->call_default_constructor();
    }
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::BeginPlay()
  {
  Super::BeginPlay();
  SK_ASSERTX(m_mind_instance_p != nullptr, a_str_format("SkookumScriptMindComponent '%S' on actor '%S' has no SkookumScript instance upon BeginPlay. This means its InitializeComponent() method was never called during initialization. Please check your initialization sequence and make sure this component gets properly initialized.", *GetName(), *GetOwner()->GetName()));
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::EndPlay(const EEndPlayReason::Type end_play_reason)
  {
  Super::EndPlay(end_play_reason);
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::UninitializeComponent()
  {
  // Delete SkookumScript instance if present
  if (m_mind_instance_p)
    {
    SK_ASSERTX(SkookumScript::get_initialization_level() >= SkookumScript::InitializationLevel_gameplay, "SkookumScript must be in gameplay mode when UninitializeComponent() is invoked.");

    delete_sk_instance();
    }

  Super::UninitializeComponent();
  }

//---------------------------------------------------------------------------------------

void USkookumScriptMindComponent::OnUnregister()
  {
  Super::OnUnregister();
  }

