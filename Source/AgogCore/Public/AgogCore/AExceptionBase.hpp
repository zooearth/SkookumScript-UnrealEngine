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
// AException base class declaration header.
//=======================================================================================

#pragma once

//=======================================================================================
// Includes
//=======================================================================================


//=======================================================================================
// Global Structures
//=======================================================================================

// Some common error/exception id values
// $Note - CReis In the future these could represent general types of exceptions and be bitwise
// or-ed with more specific exception values, so that both general and specific types of
// exceptions would be programmatically discernable.
enum eAErrId
  {
  AErrId_ok,
  AErrId_generic,  // Used on occasions where there is only one exception for a class and making extra exception ids is overkill
  AErrId_unknown,
  AErrId_low_memory,
  AErrId_invalid_request,
  AErrId_invalid_index,
  AErrId_invalid_index_range,
  AErrId_invalid_index_span,
  AErrId_invalid_datum_id,
  AErrId_not_supported,
  AErrId_os_error,
  AErrId_last = 64 // Marker for starting more specific exceptions
  };

///---------------------------------------------------------------------------------------
// Notes      All thrown exceptions should be derived from this base.
// Subclasses AException
// See Also   AEx<>, ADebug, AErrorOutputBase
// UsesLibs   
// InLibs     AgogCore/AgogCore.lib
// Examples:    
// Author(s)  Conan Reis
class AExceptionBase
  {
  public:
  // Common Methods

    AExceptionBase() {}
    virtual ~AExceptionBase() {}

  // Modifying Methods

    // This method should supply appropriate values to, call, and then return the value
    // resulting from AErrorOutputBase::get_default().determine_choice(-);
    virtual eAErrAction resolve() = 0;

  };  // AExceptionBase


//=======================================================================================
// AExceptionBase Inline Functions
//=======================================================================================

