//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript Unreal Engine Bindings
// 
// Author:  Conan Reis
//=======================================================================================

#pragma once

class SkUEBindingsInterface;

//---------------------------------------------------------------------------------------
// 
class SkUEBindings
  {
  public:

    // Class Methods

    static void ensure_static_ue_types_registered(SkUEBindingsInterface * game_generated_bindings_p);
    static void begin_register_bindings();
    static void finish_register_bindings(SkUEBindingsInterface * game_generated_bindings_p);

  }; // SkUEBindings

