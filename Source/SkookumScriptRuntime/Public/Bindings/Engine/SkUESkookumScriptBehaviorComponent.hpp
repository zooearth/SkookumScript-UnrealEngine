//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Bindings for the SkookumScriptBehaviorComponent class
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "../SkUEClassBinding.hpp"

//---------------------------------------------------------------------------------------

class SKOOKUMSCRIPTRUNTIME_API SkUESkookumScriptBehaviorComponent : public SkUEClassBindingEntity<SkUESkookumScriptBehaviorComponent, USkookumScriptBehaviorComponent>
  {
  public:

    static void       register_bindings();
    static SkClass *  get_class();

  };
