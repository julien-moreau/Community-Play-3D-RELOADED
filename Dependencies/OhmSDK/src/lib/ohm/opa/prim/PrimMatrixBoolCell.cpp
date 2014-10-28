/*****************************************************************************

        PrimMatrixBoolCell.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54714

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

#include "ohm/opa/prim/PrimMatrixBoolCell.h"
#include "ohm/opa/private/AspectWidget.h"
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

PrimMatrixBoolCell::PrimMatrixBoolCell ()
:  _state_flag (*this, &PrimMatrixBoolCell::invalidate_on_change)
,  _hit_flag (*this, &PrimMatrixBoolCell::invalidate_on_change)
{
   set_bounding_box (opa::Rect (0, 0, 16, 16));
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimMatrixBoolCell::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetMatrixBool & aspect_matrix_bool = aspect._matrix_bool;
   
   bool state_flag = _state_flag.get_cur_value ();
   bool hit_flag = _hit_flag.get_cur_value ();
   
   opak::use (aspect_matrix_bool._button_arr [state_flag ? 1 : 0][hit_flag ? 1 : 0])
      .point (0, 0)
      .scale ()
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

PrimMatrixBoolCell::~PrimMatrixBoolCell ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : invalidate_on_change
==============================================================================
*/

void  PrimMatrixBoolCell::invalidate_on_change ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
