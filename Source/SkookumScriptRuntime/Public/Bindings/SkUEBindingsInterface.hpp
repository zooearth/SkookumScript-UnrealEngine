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
// Interface class to facilitate initialization of bindings
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
