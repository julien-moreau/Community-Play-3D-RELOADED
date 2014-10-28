/*****************************************************************************

        PrimPushButtonSmall.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54712

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimPushButtonSmall.h"
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
Name: PrimPushButtonSmall
==============================================================================
*/

PrimPushButtonSmall::PrimPushButtonSmall ()
:  _type (*this, &PrimPushButtonSmall::invalidate_on_change)
,  _state (*this, &PrimPushButtonSmall::invalidate_on_change)
{
   opa::Rect rect (0, 0, 16, 16);
   rect += opa::Margin (3, 2, 3, 4);
   
   set_bounding_box (rect);
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimPushButtonSmall::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);

   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetPushButtonSmall & aspect_push_button = aspect._push_button_small;
   
   AspectWidgetPushButtonSmall::State state = _state.get_cur_value ();
   AspectWidgetPushButtonSmall::Type type = _type.get_cur_value ();
   
   opak::use (aspect_push_button._button_arr [type][state])
      .point (-3, -2)
      .scale ()
      .hint ()
      .display (context)
   ;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimPushButtonSmall
==============================================================================
*/

PrimPushButtonSmall::~PrimPushButtonSmall ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: state_changed
==============================================================================
*/

void PrimPushButtonSmall::invalidate_on_change ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

