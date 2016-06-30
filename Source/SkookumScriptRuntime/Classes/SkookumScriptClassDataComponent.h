//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2016 Agog Labs Inc. All rights reserved.
//
// Component to associate a SkookumScript class and data members with a UE4 actor
// and allow SkookumScript ctors and dtors to be called when the actor (i.e. the component) gets created/destroyed
// 
// Author: Conan Reis, Markus Breyer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "Components/ActorComponent.h"
#include "SkookumScriptClassDataComponent.generated.h"

//=======================================================================================
// Global Defines / Macros
//=======================================================================================

//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// Allows you to specify a custom SkookumScript class for this actor, to store SkookumScript data members, and to have a constructor/destructor invoked when an instance of this actor gets created/destroyed
UCLASS(classGroup=Scripting, editinlinenew, BlueprintType, meta=(BlueprintSpawnableComponent), hideCategories=(Object, ActorComponent), EarlyAccessPreview)
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

