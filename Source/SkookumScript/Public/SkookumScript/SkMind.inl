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
// SkookumScript C++ library.
//
// Mind object - tracks and updates coroutines.
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include <SkookumScript/SkClass.hpp>
#include <SkookumScript/SkInvokedCoroutine.hpp>


//=======================================================================================
// Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Get name of mind object. Most (if not all) mind objects are singletons so just borrow
// name from class.
A_INLINE const ASymbol & SkMind::get_name() const
  {
  return m_class_p->get_name();
  }

//---------------------------------------------------------------------------------------
// Stop tracking the specified invoked coroutine
//
// Author(s): Conan Reis
A_INLINE void SkMind::coroutine_track_stop(SkInvokedCoroutine * icoro_p)
  {
  icoro_p->AListNode<SkInvokedCoroutine>::remove();
  icoro_p->m_flags &= ~SkInvokedCoroutine::Flag_tracked_mask;
  }

