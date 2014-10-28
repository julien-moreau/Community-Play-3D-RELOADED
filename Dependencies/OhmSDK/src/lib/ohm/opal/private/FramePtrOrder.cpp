/*****************************************************************************

        FramePtrOrder.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48341

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

#include "ohm/opal/private/FramePtrOrder.h"

#include "ohm/lang/fnc.h"

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

FramePtrOrder::FramePtrOrder ()
:  _frame_ptr (0)
,  _resident_flag (false)
,  _order (size_t (-1))
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FramePtrOrder::FramePtrOrder (const FramePtrOrder &other)
:  _frame_ptr (other._frame_ptr)
,  _resident_flag (other._resident_flag)
,  _order (other._order)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FramePtrOrder::~FramePtrOrder ()
{
   assert (!_resident_flag);
}



/*
==============================================================================
Name : set_frame
==============================================================================
*/

void  FramePtrOrder::set_frame (Frame & frame)
{
   assert (_frame_ptr == 0);
   assert (&frame != 0);
   assert (!_resident_flag);
   
   _frame_ptr = &frame;
   _resident_flag = true;
}



/*
==============================================================================
Name : set_order
==============================================================================
*/

void  FramePtrOrder::set_order (size_t order)
{
   assert (_resident_flag);

   _order = order;
}



/*
==============================================================================
Name : get_frame_ptr
==============================================================================
*/

Frame *  FramePtrOrder::get_frame_ptr ()
{
   assert (_frame_ptr != 0);
   assert (_resident_flag);
   
   return _frame_ptr;
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  FramePtrOrder::remove ()
{
   assert (_resident_flag);
   
   _resident_flag = false;
   _frame_ptr = 0;
}



/*
==============================================================================
Name : was_removed
==============================================================================
*/

bool  FramePtrOrder::was_removed () const
{
   return !_resident_flag;
}



/*
==============================================================================
Name : operator <
Note :
   Not necessarily total
==============================================================================
*/

bool  FramePtrOrder::operator < (const FramePtrOrder & other) const
{
   // 'this' frame or the 'other' might have been marked as
   // removable from the container, but are still in it
   // the iterators are going to be erased in the FrameBin
   
   // we therefore don't test for residency, as this is not
   // going to produce glitch in the event propagation
   
   int c = lang::cmp (_order, other._order);
   return c < 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
