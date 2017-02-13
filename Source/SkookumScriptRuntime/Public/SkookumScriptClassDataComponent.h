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
// Component to associate a SkookumScript class and data members with a UE4 actor
// and allow SkookumScript ctors and dtors to be called when the actor (i.e. the component) gets created/destroyed
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "Components/ActorComponent.h"

#include <SkookumScript/SkInstance.hpp>

#include "SkookumScriptClassDataComponent.generated.h"

//=======================================================================================
// Global Defines / Macros
//=======================================================================================

//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// Allows you to specify a custom SkookumScript class for this actor, to store SkookumScript data members, and to have a constructor/destructor invoked when an instance of this actor gets created/destroyed
UCLASS(classGroup=Scripting, editinlinenew, BlueprintType, meta=(BlueprintSpawnableComponent), hideCategories=(Object, ActorComponent))
class SKOOKUMSCRIPTRUNTIME_API USkookumScriptClassDataComponent : public UActorComponent
  {

    GENERATED_UCLASS_BODY()

  public:

  // Public Data Members

    // SkookumScript class type of the owner actor - used to create appropriate SkookumScript actor instance.
    // Uses class of this actor's Blueprint if left blank.
    UPROPERTY(Category = Script, EditAnywhere, BlueprintReadOnly)
    FString ScriptActorClassName;

  // Methods

    // Gets our SkookumScript instance
    SkInstance * get_sk_actor_instance() const { return m_actor_instance_p; }

  protected:

    // Begin UActorComponent interface
    virtual void OnRegister() override;
    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type end_play_reason) override;
    virtual void UninitializeComponent() override;
    virtual void OnUnregister() override;

    // Creates/deletes our SkookumScript instance
    void        create_sk_instance();
    void        delete_sk_instance();

    // Keep the SkookumScript instance belonging to this actor around
    AIdPtr<SkInstance> m_actor_instance_p;

  };  // USkookumScriptClassDataComponent


//=======================================================================================
// Inline Functions
//=======================================================================================

