//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript Name (= FName) class
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkClassBinding.hpp>
#include <NameTypes.h>

//---------------------------------------------------------------------------------------
// SkookumScript Name (= FName) class
class SKOOKUMSCRIPTRUNTIME_API SkUEName : public SkClassBindingSimple<SkUEName, FName>
  {
  public:

    enum { Binding_has_ctor = false }; // Do not auto-generate constructor since we have a special one taking a String argument

    static void       register_bindings();
    static SkClass *  get_class();

  };
