/*****************************************************************************

        PrimDialog.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54636

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

#include "ohm/opa/prim/PrimDialog.h"
#include "ohm/opa/private/AspectWidget.h"
//#include  "ohm/opal/GraphicContextInterface.h"
//#include "ohm/opak/base/PrimitiveHelper.h"

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

PrimDialog::PrimDialog ()
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimDialog::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetDialog & aspect_dialog = aspect._dialog;
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect (
      -10, -5,
      size._width + 10 + 10,
      30
   );
   
   opak::use (aspect_dialog._header)
      .rect (rect)
      .scale_border ()
      .hint ()
      .display (context)
   ;
   
   rect._origin._y += rect._size._height;
   rect._size._height = size._height - 25 - 50;
   
   opak::use (aspect_dialog._content)
      .rect (rect)
      .scale_border ()
      .hint ()
      .display (context)
   ;

   rect._origin._y += rect._size._height;
   rect._size._height = 50 + 18;

   opak::use (aspect_dialog._footer)
      .rect (rect)
      .scale_border ()
      .hint ()
      .display (context)
   ;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimDialog::~PrimDialog ()
{
}



/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void  PrimDialog::do_size_changed ()
{
   // add shadows
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect;
   rect._size = size;
   
   rect._origin = opa::Point (-10, -5);
   rect._size += opa::Size (10, 5);
   rect._size += opa::Size (10, 18);
   
   set_bounding_box (rect);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
