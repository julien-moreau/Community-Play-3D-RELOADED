/*****************************************************************************

        Bitmap.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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
#include "ohm/opak/Bitmap.h"
#include "ohm/opa/Quad.h"
#include "ohm/lang/fnc.h"

#include <cassert>



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

Bitmap::Bitmap ()
:  _bitmap (*this, &Bitmap::bitmap_changed)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  Bitmap::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   Opacifiable::Auto auto_opacifiable (*this, context);
   BlendModable::Auto auto_blend_modable (*this, context);
   
   opal::Image image (context.get (), _bitmap.get_cur_value ());

   opa::Point pt_0 (0.f, 0.f);
   opa::Point pt_1 (float (image._width), 0.f);
   opa::Point pt_2 (float (image._width), float (image._height));
   opa::Point pt_3 (0.f, float (image._height));

   context.apply (pt_0);
   context.apply (pt_1);
   context.apply (pt_2);
   context.apply (pt_3);
   
   opa::Quad vertices (pt_0, pt_1, pt_2, pt_3);
   opa::Quad tex_coords (image);
   
   context.get ().draw (image, vertices, tex_coords);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Bitmap::~Bitmap ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bitmap_changed
==============================================================================
*/

void  Bitmap::bitmap_changed ()
{
   const opal::Bitmap & bitmap = _bitmap.get_cur_value ();
   
   opa::Size size (float (bitmap.get_width ()), float (bitmap.get_height ()));
   
   set_bounding_box (opa::Rect (opa::Point::_zero, size));
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
