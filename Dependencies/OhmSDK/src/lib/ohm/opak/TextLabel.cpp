/*****************************************************************************

        TextLabel.cpp
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
#include "ohm/opal/AutoContextState.h"
#include "ohm/opal/private/Clock.h"
#include "ohm/opak/TextLabel.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/util/trace.h"

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

TextLabel::TextLabel ()
:  _scripture (*this, &TextLabel::scripture_changed)
,  _flush_factor (*this, &TextLabel::flush_factor_changed)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  TextLabel::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   
   float flush_factor = _flush_factor.get_cur_value ();
   
   opak::PrimitiveHelper::display (
      context,
      _label_string,
      opa::Point::_zero,
      flush_factor
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TextLabel::~TextLabel ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : scripture_changed
==============================================================================
*/

void  TextLabel::scripture_changed ()
{
   const opal::Scripture & scripture = _scripture.get_cur_value ();
   
   _label_string.begin_edit ();
   _label_string.use_scripture () = scripture;
   _label_string.use_scripture ().convert_fonts ();
   _label_string.end_edit ();
      
   update_bbox ();
}



/*
==============================================================================
Name : flush_factor_changed
==============================================================================
*/

void  TextLabel::flush_factor_changed ()
{
   update_bbox ();
}



/*
==============================================================================
Name : update_bbox
==============================================================================
*/

void  TextLabel::update_bbox ()
{
   float flush_factor = _flush_factor.get_cur_value ();
   
   const opa::Rect & typographic_bounds = _label_string.get_typographic_bounds ();
   float flush_offset = - floor (flush_factor * typographic_bounds._size._width);

   opa::Rect image_bounds = _label_string.get_image_bounds ();
   
   image_bounds._origin._x += flush_offset;
   
   set_bounding_box (image_bounds);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
