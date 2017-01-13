//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript transform (position + rotation + scale) class
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

#include "Math/UnrealMath.h" // Vector math functions
#include <SkookumScript/SkClassBinding.hpp>

//---------------------------------------------------------------------------------------
// SkookumScript transform (position + rotation + scale) class
class SKOOKUMSCRIPTRUNTIME_API SkTransform : public SkClassBindingSimple<SkTransform, FTransform>
  {
  public:

    static void       register_bindings();
    static SkClass *  get_class();

  };
