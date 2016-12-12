//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript Unreal Engine Bindings
// 
// Author:  Conan Reis
//=======================================================================================

#pragma once

#include "../SkookumScriptRuntimePrivatePCH.h"

class SkUEBindingsInterface;

//---------------------------------------------------------------------------------------
// 
class SkUEBindings
  {
  public:

    // Class Methods

    static void register_static_ue_types(SkUEBindingsInterface * game_generated_bindings_p);
    static void register_all_bindings(SkUEBindingsInterface * game_generated_bindings_p);

  }; // SkUEBindings

