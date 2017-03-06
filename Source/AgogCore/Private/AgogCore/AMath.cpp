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
// Math Common Constants & Scalar Function Definitions
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AgogCore.hpp> // Always include AgogCore first (as some builds require a designated precompiled header)
#include <AgogCore/AMath.hpp>
#ifdef A_INL_IN_CPP
  #include <AgogCore/AMath.inl>
#endif


//=======================================================================================
// Local Macros / Defines
//=======================================================================================

namespace
{

  eAYaw g_yaws[8] =
    {
    AYaw_up,
    AYaw_up_right,
    AYaw_right,
    AYaw_down_right,
    AYaw_down,
    AYaw_down_left,
    AYaw_left,
    AYaw_up_left
    };

} // End unnamed namespace


//=======================================================================================
// Functions
//=======================================================================================

//---------------------------------------------------------------------------------------
// Converts a radian angle to a discrete yaw direction.
// Returns:    an eAYaw value.  All the directions can be used including diagonals or
//             just the cardinal directions of up, right, down, and left can.
// Arg         rads - angle in radians or -1.0 to indicate no yaw or "centered".
// See:        eAYaw, a_yaw_to_angle()
// Author(s):   Conan Reis
eAYaw a_angle_to_yaw(f32 rads)
  {
  return (rads != -1.0f)
    ? g_yaws[a_floor((rads + A_22_5_deg) / A_45_deg) % 8]
    : AYaw_none;
  }

//---------------------------------------------------------------------------------------
// Converts a discrete yaw direction to a radian angle
// Returns:    a radian angle from 0 to 2pi or -1.0 if the yaw was none (centered).
// Arg         yaw - direction - see eAYaw
// See:        eAYaw, a_angle_to_yaw()
// Author(s):   Conan Reis
f32 a_yaw_to_angle(eAYaw yaw)
  {
  switch (yaw)
    {
    case AYaw_up:         return A_0_deg;
    case AYaw_up_right:   return A_45_deg;
    case AYaw_right:      return A_90_deg;
    case AYaw_down_right: return A_135_deg;
    case AYaw_down:       return A_180_deg;
    case AYaw_down_left:  return A_225_deg;
    case AYaw_left:       return A_270_deg;
    case AYaw_up_left:    return A_315_deg;
    default:              return -1.0f;
    }
  }
