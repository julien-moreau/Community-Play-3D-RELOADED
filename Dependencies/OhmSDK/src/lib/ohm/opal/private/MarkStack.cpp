/*****************************************************************************

        MarkStack.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52274

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

#include "ohm/opal/private/MarkStack.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

MarkStack::MarkStack ()
:  _type_info_id_arr ()
,  _pos_arr ()
{
   reset ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MarkStack::~MarkStack ()
{
   assert (_pos_arr.size () == 1);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

MarkStack & MarkStack::operator = (const MarkStack & other)
{
   _type_info_id_arr = other._type_info_id_arr;
   _pos_arr = other._pos_arr;
   
   return *this;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void  MarkStack::reset ()
{
   _type_info_id_arr.clear ();
   _pos_arr.clear ();
   _pos_arr.push_back (0);
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  MarkStack::save ()
{
   _pos_arr.push_back (_type_info_id_arr.size ());
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  MarkStack::restore ()
{
   assert (_pos_arr.size () > 1);
   
   size_t pos = _pos_arr.back ();
   
   _type_info_id_arr.resize (pos);
   
   _pos_arr.pop_back ();
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  MarkStack::set (const std::type_info * info_ptr, archi::UInt32 mark_id)
{
   TypeInfoId type_info_id;
   type_info_id._info_ptr = info_ptr;
   type_info_id._id = mark_id;
   type_info_id._stack_size = _pos_arr.size () - 1;
   
   _type_info_id_arr.push_back (type_info_id);
}



/*
==============================================================================
Name : has
Description :
==============================================================================
*/

bool  MarkStack::has (archi::UInt32 mark_id) const
{
   TypeInfoIdArr::const_reverse_iterator it = _type_info_id_arr.rbegin ();
   const TypeInfoIdArr::const_reverse_iterator it_end = _type_info_id_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      const TypeInfoId & type_info_id = *it;
      
      if (type_info_id._id == mark_id)
      {
         return true;
      }
   }
   
   return false;
}



/*
==============================================================================
Name : find
Description :
==============================================================================
*/

size_t   MarkStack::find (archi::UInt32 mark_id) const
{
   TypeInfoIdArr::const_reverse_iterator it = _type_info_id_arr.rbegin ();
   const TypeInfoIdArr::const_reverse_iterator it_end = _type_info_id_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      const TypeInfoId & type_info_id = *it;
      
      if (type_info_id._id == mark_id)
      {
         return type_info_id._stack_size;
      }
   }
   
   assert (false);
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
