/*****************************************************************************

        PrimWidgetBaseDebug.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54597

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

#include "ohm/opa/private/PrimWidgetBaseDebug.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opak/base/PrimitiveHelper.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimWidgetBaseDebug::PrimWidgetBaseDebug ()
:  _min_bbox (*this, &PrimWidgetBaseDebug::invalidate_on_change)
,  _margin (*this, &PrimWidgetBaseDebug::invalidate_on_change)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimWidgetBaseDebug::display (opak::GraphicContext & context)
{
   Primitive::Auto auto_prim_widget (*this, context);
   
   context.get ().set_blend_mode_max ();
   
   stroke (
      context,
      Rect (Point::_zero, _size.get_cur_value ()),
      opal::Color (1.f, 1.f, 0.f, 0.f)
   );
   
   Rect min_bbox (Point::_zero, _min_bbox._size.get_cur_value ());
   
   stroke (
      context,
      min_bbox,
      opal::Color (1.f, 0.f, 1.f, 0.f)
   );
   
   opak::PrimitiveHelper::display (
      context,
      Rect (
         Point (- _min_bbox._origin.get_cur_value ()),
         Size (1, 1)
      ),
      opal::Color (1.f, 0.f, 1.f, 0.f),
      true
   );
   
   Rect margined = min_bbox;
   margined += _margin.get_cur_value ();
   
   stroke (
      context,
      margined,
      opal::Color (1.f, 0.f, 0.f, 1.f)
   );
   
   context.get ().set_blend_mode_normal ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimWidgetBaseDebug::~PrimWidgetBaseDebug ()
{
}



/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void  PrimWidgetBaseDebug::do_size_changed ()
{
   update_bbox ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : invalidate_on_change
==============================================================================
*/

void  PrimWidgetBaseDebug::invalidate_on_change ()
{
   update_bbox ();
}



/*
==============================================================================
Name : stroke
Note : interior
==============================================================================
*/

void  PrimWidgetBaseDebug::stroke (opak::GraphicContext & context, const Rect & rect, const opal::Color & color)
{
   opak::PrimitiveHelper::display (
      context,
      Rect (rect._origin, Size (rect._size._width, 1)),
      color,
      true
   );
   
   opak::PrimitiveHelper::display (
      context,
      Rect (rect._origin, Size (1, rect._size._height)),
      color,
      true
   );

   opak::PrimitiveHelper::display (
      context,
      Rect (Point (0, rect._size._height - 1), Size (rect._size._width, 1)),
      color,
      true
   );
   
   opak::PrimitiveHelper::display (
      context,
      Rect (Point (rect._size._width - 1, 0), Size (1, rect._size._height)),
      color,
      true
   );
}



/*
==============================================================================
Name : update_bbox
==============================================================================
*/

void  PrimWidgetBaseDebug::update_bbox ()
{
   Rect bbox_1 (Point::_zero, _size.get_cur_value ());
   
   Rect bbox_2 = _min_bbox.get_cur_value ();
   bbox_2._origin = Point::_zero;
   bbox_2 += _margin.get_cur_value ();
   
   Rect bbox = bbox_1 | bbox_2;
   
   set_bounding_box (bbox);
   
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
