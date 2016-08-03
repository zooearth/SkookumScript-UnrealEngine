//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Interface class to facilitate initialization of bindings
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================

//---------------------------------------------------------------------------------------
// Interface class to facilitate initialization of bindings
class SkUEBindingsInterface
  {
  public:

    // Cache pointers to UE4 types (UClass, UStruct, UEnum) that we provide Sk bindings for
    // This happens only once when the game starts up
    virtual void register_static_ue_types() = 0;

    // (Re-)cache pointers to Sk types (SkClass) and map them to UE4 types (this 
    // This happens repeatedly as the SkookumScript compiled binaries get reloaded
    virtual void register_static_sk_types() = 0;

    // Register pointers to atomic C++ methods and coroutines with SkookumScript
    // This also happens repeatedly as the SkookumScript compiled binaries get reloaded
    virtual void register_bindings() = 0;

  };
