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

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "Components/ActorComponent.h"

#include "SkookumScriptConstructionComponent.generated.h"

//=======================================================================================
// Global Defines / Macros
//=======================================================================================

//=======================================================================================
// Global Structures
//=======================================================================================

//---------------------------------------------------------------------------------------
// Allows you to specify a custom SkookumScript class for this actor, to store SkookumScript data members, and to have a constructor/destructor invoked when an instance of this actor gets created/destroyed
UCLASS()
class USkookumScriptConstructionComponent : public UActorComponent
  {

    GENERATED_UCLASS_BODY()

  protected:

    // Begin UActorComponent interface
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;

  };  // USkookumScriptConstructionComponent

