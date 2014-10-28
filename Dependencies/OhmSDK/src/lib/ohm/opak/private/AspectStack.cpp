/*****************************************************************************

        AspectStack.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38607

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

#include "ohm/opak/private/AspectStack.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

AspectStack::AspectStack ()
:  _type_info_ptr_arr ()
,  _pos_arr ()
{
   reset ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectStack::~AspectStack ()
{
   assert (_pos_arr.size () == 1);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

AspectStack &  AspectStack::operator = (const AspectStack & other)
{
   _type_info_ptr_arr = other._type_info_ptr_arr;
   _pos_arr = other._pos_arr;
   
   return *this;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void  AspectStack::reset ()
{
   _type_info_ptr_arr.clear ();
   _pos_arr.clear ();
   _pos_arr.push_back (0);
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  AspectStack::save ()
{
   size_t size = _type_info_ptr_arr.size ();
   
   _pos_arr.push_back (size);
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  AspectStack::restore ()
{
   assert (_pos_arr.size () > 1);
   
   size_t size = _pos_arr.back ();
   
   _type_info_ptr_arr.resize (size);
   
   _pos_arr.pop_back ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
