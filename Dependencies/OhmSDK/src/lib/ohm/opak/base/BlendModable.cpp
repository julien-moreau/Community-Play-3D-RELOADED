/*****************************************************************************

        BlendModable.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 32023

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

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opak/base/BlendModable.h"
#include "ohm/lang/fnc.h"

#include <cassert>
#include <string>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

BlendModable::BlendModable ()
:  _blend_mode (*this, &BlendModable::blend_mode_changed)
{
   _blend_mode.init (BlendMode_NORMAL);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BlendModable::~BlendModable ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Auto::ctor
==============================================================================
*/

BlendModable::Auto::Auto (BlendModable & primitive, GraphicContext & context)
:  _primitive (primitive)
,  _context (context)
{
   _primitive.BlendModable::pre_display (_context);
}



/*
==============================================================================
Name : Auto::dtor
==============================================================================
*/

BlendModable::Auto::~Auto ()
{
   _primitive.BlendModable::post_display (_context);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : pre_display
==============================================================================
*/

void  BlendModable::pre_display (GraphicContext & context)
{
   BlendMode blend_mode = _blend_mode.get_cur_value ();
   
   switch (blend_mode)
   {
   case BlendMode_NORMAL:
      context.get ().set_blend_mode_normal ();
      break;

   case BlendMode_MULTIPLY:
      context.get ().set_blend_mode_multiply ();
      break;

   case BlendMode_SCREEN:
      context.get ().set_blend_mode_screen ();
      break;
      
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : post_display
==============================================================================
*/

void  BlendModable::post_display (GraphicContext & context)
{
   BlendMode blend_mode = _blend_mode.get_cur_value ();
   
   if (blend_mode != BlendMode_NORMAL)
   {
      context.get ().set_blend_mode_normal ();
   }
}



/*
==============================================================================
Name : blend_mode_changed
==============================================================================
*/

void  BlendModable::blend_mode_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
