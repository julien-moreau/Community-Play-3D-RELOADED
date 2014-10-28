/*****************************************************************************

        PrimitivePtrOrder.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58534

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

#include "ohm/opak/private/PrimitivePtrOrder.h"
#include "ohm/opak/base/Primitive.h"

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

PrimitivePtrOrder::PrimitivePtrOrder (Primitive & primitive)
:  _primitive_ptr (&primitive)
,  _order (size_t (-1))
,  _state (State_RESIDENT)
{
   assert (_primitive_ptr != 0);
   
   _primitive_ptr->inc_ref_cnt ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

PrimitivePtrOrder::PrimitivePtrOrder (const PrimitivePtrOrder &other)
:  _primitive_ptr (other._primitive_ptr)
,  _order (other._order)
,  _state (other._state)
{
   assert (_primitive_ptr != 0);
   
   _primitive_ptr->inc_ref_cnt ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimitivePtrOrder::~PrimitivePtrOrder ()
{
   // will not throw
   _primitive_ptr->dec_ref_cnt ();
}



/*
==============================================================================
Name : set_order
==============================================================================
*/

void  PrimitivePtrOrder::set_order (size_t order)
{
   _order = order;
}



/*
==============================================================================
Name : get_primitive_ptr
==============================================================================
*/

Primitive * PrimitivePtrOrder::get_primitive_ptr ()
{
   assert (_primitive_ptr != 0);
   
   return _primitive_ptr;
}



/*
==============================================================================
Name : kill
==============================================================================
*/

void  PrimitivePtrOrder::kill ()
{
   //assert (_state == State_RESIDENT);
   
   _state = State_DYING;
}



/*
==============================================================================
Name : resurrect
==============================================================================
*/

void  PrimitivePtrOrder::resurrect ()
{
   //assert (_state == State_DYING);
   
   _state = State_RESIDENT;
}



/*
==============================================================================
Name : is_dying
==============================================================================
*/

bool  PrimitivePtrOrder::is_dying () const
{
   return _state == State_DYING;
}



/*
==============================================================================
Name : operator <
Note :
   Not necesserally total
==============================================================================
*/

bool  PrimitivePtrOrder::operator < (const PrimitivePtrOrder & other) const
{
   return _order < other._order;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
