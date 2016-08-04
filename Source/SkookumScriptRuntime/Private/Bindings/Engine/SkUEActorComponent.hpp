//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Additional bindings for the ActorComponent (= UActorComponent) class 
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <SkUEGeneratedBindings.generated.hpp>

//=======================================================================================
// Global Functions
//=======================================================================================

//---------------------------------------------------------------------------------------
// Bindings for the Actor (= AActor) class 
class SkUEActorComponent_Ext : public SkUEActorComponent
  {
  public:
    static void register_bindings();
  };

