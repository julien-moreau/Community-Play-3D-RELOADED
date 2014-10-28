/*****************************************************************************

        GraphicStateStack.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 26966

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

#include "ohm/opal/private/GraphicStateStack.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

GraphicStateStack::GraphicStateStack ()
:  _size (0)
,  _graphic_state_arr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

GraphicStateStack::~GraphicStateStack ()
{
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   GraphicStateStack::size () const
{
   return _size;
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  GraphicStateStack::push (const GraphicState & graphic_state)
{
   _size += 1;
   
   // graphic state might be a reference to a state
   // in the current stack...
   GraphicState graphic_state_copy = graphic_state;
   
   if (_size > _graphic_state_arr.size ())
   {
      _graphic_state_arr.resize (_size);
   }
   
   assert (_size - 1 < _graphic_state_arr.size ());
   
   _graphic_state_arr [_size - 1] = graphic_state_copy;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  GraphicStateStack::pop ()
{
   assert (_size > 0);
   
   _size -= 1;
}



/*
==============================================================================
Name : top
==============================================================================
*/

GraphicState & GraphicStateStack::top ()
{
   assert (_size - 1 < _graphic_state_arr.size ());
   
   return _graphic_state_arr [_size - 1];
}



/*
==============================================================================
Name : top
==============================================================================
*/

const GraphicState & GraphicStateStack::top () const
{
   assert (_size - 1 < _graphic_state_arr.size ());
   
   return _graphic_state_arr [_size - 1];
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
