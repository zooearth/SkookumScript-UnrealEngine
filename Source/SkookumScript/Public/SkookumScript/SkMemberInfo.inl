
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// Member Identifier class
// Author(s):   Conan Reis
// Notes:          
//=======================================================================================


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
// Includes
//=======================================================================================


//=======================================================================================
// SkMemberInfo Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// Equals operator
// Returns:    true if logically equal, false if not
// Author(s):   Conan Reis
A_INLINE bool SkMemberInfo::operator==(const SkMemberInfo & info) const
  {
  return ((m_type == info.m_type)
    && ((m_type == SkMember__invalid)
      || ((m_class_scope == info.m_class_scope) /*&& (m_is_closure == info.m_is_closure)*/ && (m_member_id == info.m_member_id)))); 
  }

//---------------------------------------------------------------------------------------
// Less than operator - sort by type -> member_id(scope name -> name) -> class scope
// Returns:    true if logically less than, false if not
// Author(s):   Conan Reis
A_INLINE bool SkMemberInfo::operator<(const SkMemberInfo & info) const
  {
  return ((m_type < info.m_type)
       || ((m_type == info.m_type)
         && (m_type != SkMember__invalid)
         && (m_member_id.less_ids_scope_name(info.m_member_id)
           || (m_member_id.equal_ids_scope_name(info.m_member_id)
             && ((m_class_scope < info.m_class_scope)
               /*|| (m_class_scope == info.m_class_scope
                 && (m_is_closure < info.m_is_closure))*/)))));
  }

