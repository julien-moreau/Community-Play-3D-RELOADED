/*****************************************************************************

        SeedGenerator.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55766

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/SeedGenerator.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

SeedGenerator::~SeedGenerator ()
{
}



/*
==============================================================================
Name : get
Description :
   Get the starting point (seed) of the reference system.
==============================================================================
*/

archi::Int64   SeedGenerator::get (archi::Int32 user_id, archi::Int32 thread_id)
{
   // this limits the number of simultaneous user to 1 ^ NBR_USER_BITS - 1

   assert (user_id >= 0);
   assert (user_id <= EDITOR_USER_ID_MAX);
   
   // this limits the number of threads per user to 1 ^ NBR_THREAD_BITS

   assert (thread_id >= 0);
   assert (thread_id <= NBR_THREAD_MAX);

   // ... but enlarge our memory pools a lot, as the pools management
   // are dumb for now (only incrementation)
   
   archi::Int64 ret_val = user_id;
   ret_val <<= NBR_THREAD_BITS;
   
   ret_val |= thread_id;
   
   ret_val <<= 64LL - archi::Int64 (SIGN_BIT + NBR_USER_BITS + NBR_THREAD_BITS);
   
   // start at 1. 0 is reserved as we need to make a difference
   // between NULL and the first object of the document writer.
   
   ret_val += 1LL;
   
   return ret_val;
}



/*
==============================================================================
Name : to_user_id
Description :
   Returns the user_id deduced from the value.
==============================================================================
*/

archi::Int32   SeedGenerator::to_user_id (archi::Int64 val)
{
   if (val < 0)
   {
      val = - val;
   }

   val >>= 64LL - archi::Int64 (SIGN_BIT + NBR_USER_BITS);
   
   val &= (1LL << NBR_USER_BITS) - 1LL;
   
   return archi::Int32 (val);
}



/*
==============================================================================
Name : to_thread_id
Description :
   Returns the thread_id deduced from the value.
==============================================================================
*/

archi::Int32   SeedGenerator::to_thread_id (archi::Int64 val)
{
   if (val < 0)
   {
      val = - val;
   }

   val >>= 64LL - archi::Int64 (SIGN_BIT + NBR_USER_BITS + NBR_THREAD_BITS);
   
   val &= (1LL << NBR_THREAD_BITS) - 1LL;
   
   return archi::Int32 (val);
}



/*
==============================================================================
Name : to_thread_id
Description :
   Returns the obj id (excluding sign, user id and thread id) deduced from
   the value.
==============================================================================
*/

archi::Int64   SeedGenerator::to_per_user_per_thread_obj_id (archi::Int64 val)
{
   val &=
      (
         1LL << (
            64LL - archi::Int64 (SIGN_BIT + NBR_USER_BITS + NBR_THREAD_BITS)
         )
      )
      - 1LL;
   
   return val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

SeedGenerator::SeedGenerator ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
