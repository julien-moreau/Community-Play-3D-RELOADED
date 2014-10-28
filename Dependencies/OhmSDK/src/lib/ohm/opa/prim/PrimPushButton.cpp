/*****************************************************************************

        PrimPushButton.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70452

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

#include "ohm/opa/prim/PrimPushButton.h"
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
Name: PrimPushButton
==============================================================================
*/

PrimPushButton::PrimPushButton ()
:  _state (*this, &PrimPushButton::state_changed)
{
   _state.init (State_NORMAL);
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimPushButton::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);

   const opa::Size size = _size.get_cur_value ();

   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetPushButton & aspect_push_button = aspect._push_button;
   
   opa::Rect rect;
   rect._size = _size.get_cur_value ();
   
   State state = _state.get_cur_value ();
   
   AspectWidgetPushButton::State pstate = AspectWidgetPushButton::State_OFF;
   
   switch (state)
   {
   case State_ROLLOVER:
      pstate = AspectWidgetPushButton::State_OVER;
      break;

   case State_PRESSED:
      pstate = AspectWidgetPushButton::State_ON;
      break;
   
   default:
      break;
   }
   
   opak::use (aspect_push_button._button_arr [pstate])
      .rect (rect)
      .margin (4, 3, 4, 6)
      .scale_border ()
      .hint ()
      .display (context)
   ;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimPushButton
==============================================================================
*/

PrimPushButton::~PrimPushButton ()
{
}



/*
==============================================================================
Name: do_size_changed
==============================================================================
*/

void PrimPushButton::do_size_changed ()
{
   // add shadows
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect;
   rect._size = size;
   
   rect += opa::Margin (4, 3, 4, 6);
   
   set_bounding_box (rect);
}



/*
==============================================================================
Name: do_enabled_flag_changed
==============================================================================
*/

void PrimPushButton::do_enabled_flag_changed ()
{
   state_changed ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: state_changed
==============================================================================
*/

void PrimPushButton::state_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

