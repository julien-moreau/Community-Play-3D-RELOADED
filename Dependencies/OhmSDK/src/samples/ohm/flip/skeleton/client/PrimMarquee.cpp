/*****************************************************************************

        PrimMarquee.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70481

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

#include "client/PrimMarquee.h"

#include "ohm/opal/GraphicContextInterface.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimMarquee::PrimMarquee ()
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimMarquee::display (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   opa::Size size = _size.get_cur_value ();
   
   const opa::Point inv_px = context.get_inv_px ();
   
   float inv_norm2_x = inv_px._x;
   float inv_norm2_y = inv_px._y;
   
   if (
      (lang::abs (size._width) < 2.f * inv_norm2_x)
      || (lang::abs (size._height) < 2.f * inv_norm2_y)
      )
   {
      return;
   }
   
   Primitive::Auto auto_primitive (*this, context);
   
   if (size._width < 0.f)
   {
      inv_norm2_x = - inv_norm2_x;
   }

   if (size._height < 0.f)
   {
      inv_norm2_y = - inv_norm2_y;
   }
   
   bool hinted_flag = true;
   
   {
      context.get ().set_blend_mode_screen ();
      
      static opal::Color color (.5, .5f);
      
      opak::use (opa::Rect (1, 1, size._width - 2, size._height - 2))
         .color (color)
         .display (context);
      
      context.get ().set_blend_mode_normal ();
   }
   
   {
      context.get ().set_blend_mode_multiply ();
      
      static opal::Color color (.5f, .25f);
      
      opak::use (opa::Rect (0, 0, size._width, 1))
         .color (color)
         .display (context);

      opak::use (opa::Rect (0, size._height - 1, size._width, 1))
         .color (color)
         .display (context);

      opak::use (opa::Rect (0, 1, 1, size._height - 2))
         .color (color)
         .display (context);

      opak::use (opa::Rect (size._width - 1, 1, 1, size._height - 2))
         .color (color)
         .display (context);
      
      context.get ().set_blend_mode_normal ();
   }
   
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimMarquee::~PrimMarquee ()
{
}



/*
==============================================================================
Name: do_size_changed
==============================================================================
*/

void PrimMarquee::do_size_changed ()
{
   using namespace ohm;
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect (opa::Point::_zero, size);
   
   rect = rect.standardize ();
   
   set_bounding_box (rect, opa::Margin (2.f, 2.f, 2.f, 2.f));
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
