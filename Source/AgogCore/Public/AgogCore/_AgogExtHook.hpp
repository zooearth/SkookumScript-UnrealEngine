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
// Agog Labs C++ library.
//
// File:        External Overrides for AgogCore - used when Agog libraries hooked into
//				other code bases.
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================



//=======================================================================================
// Global Macros / Defines
//=======================================================================================

// Indicates that custom memory allocation functions for new/delete are defined in
// external libraries.
#define A_MEMORY_FUNCS_PRESENT

// Show work-in-progress notes via  #pragma A_NOTE()
//#define A_SHOW_NOTES
