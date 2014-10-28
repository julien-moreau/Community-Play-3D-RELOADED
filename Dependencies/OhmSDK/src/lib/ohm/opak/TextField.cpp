/*****************************************************************************

        TextField.cpp
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
#include "ohm/opak/TextField.h"
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

TextField::TextField ()
:  _scripture (*this, &TextField::scripture_changed)
,  _caret_first (*this, &TextField::caret_position_changed)
,  _caret_second (*this, &TextField::caret_position_changed)
,  _caret_color (*this, &TextField::caret_color_changed)
,  _selection_color (*this, &TextField::selection_color_changed)

,  _area_string ()
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  TextField::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);

   const opal::Scripture & scripture = _scripture.get_cur_value ();
   int caret_first = _caret_first.get_cur_value ();
   int caret_second = _caret_second.get_cur_value ();
   opal::Color caret_color = _caret_color.get_cur_value ();
   opal::Color selection_color = _selection_color.get_cur_value ();
   
   _area_string.begin_edit ();
   _area_string.use_scripture () = scripture;
   _area_string.use_scripture ().convert_fonts ();
   _area_string.end_edit ();
   
   PrimitiveHelper::display (
      context, _area_string, opa::Point::_zero,
      caret_first, caret_second,
      caret_color, selection_color
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TextField::~TextField ()
{
}



/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void  TextField::do_size_changed ()
{
   opa::Size size = _size.get_cur_value ();
   
   _area_string.set_bounds (size);
   
   update_bbox ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : font_changed
==============================================================================
*/

void  TextField::scripture_changed ()
{
   update_bbox ();
   
   invalidate_all ();
}



/*
==============================================================================
Name : caret_position_changed
==============================================================================
*/

void  TextField::caret_position_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : caret_color_changed
==============================================================================
*/

void  TextField::caret_color_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : selection_color_changed
==============================================================================
*/

void  TextField::selection_color_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : update_bbox
==============================================================================
*/

void  TextField::update_bbox ()
{
   const opa::Size & size = _size.get_cur_value ();
   const opal::Scripture & scripture = _scripture.get_cur_value ();
   
   _area_string.begin_edit ();
   _area_string.use_scripture () = scripture;
   _area_string.use_scripture ().convert_fonts ();
   _area_string.end_edit ();
   
   opa::Rect image_bounds = _area_string.get_image_bounds ();
   
   image_bounds |= opa::Rect (opa::Point::_zero, size);
   
   set_bounding_box (image_bounds);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
