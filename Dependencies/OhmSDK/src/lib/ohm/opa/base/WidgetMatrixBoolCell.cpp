/*****************************************************************************

        WidgetMatrixBoolCell.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53326

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

#include "ohm/opa/base/WidgetMatrixBoolCell.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opa/prim/PrimCheckBox.h"
#include "ohm/opal/base/FrameSignal.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetMatrixBoolCell::~WidgetMatrixBoolCell ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : handle_event_default
==============================================================================
*/

void  WidgetMatrixBoolCell::handle_event_default (Event & event)
{
   set_state (event._suggested_state_flag);
}



/*
==============================================================================
Name : set_state
==============================================================================
*/

void  WidgetMatrixBoolCell::set_state (bool state_flag)
{
   _state_flag = state_flag;
   
   _prim_aptr->_state_flag = state_flag;
}



/*
==============================================================================
Name : get_state
==============================================================================
*/

bool  WidgetMatrixBoolCell::get_state () const
{
   return _state_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMatrixBoolCell::WidgetMatrixBoolCell ()
:  _frame_aptr ()
,  _prim_aptr ()
,  _state_flag (false)
{
   _frame_aptr = new opal::FrameSignal;
   
   _frame_aptr->bind_gesture_trip <
      WidgetMatrixBoolCell,
      &WidgetMatrixBoolCell::req_gesture_entered,
      &WidgetMatrixBoolCell::req_gesture_exited
   > (*this);

   _frame_aptr->bind_gesture_signal <
      WidgetMatrixBoolCell, &WidgetMatrixBoolCell::req_gesture_signal
   > (*this);
   
   _prim_aptr = new PrimMatrixBoolCell;
   _prim_aptr->set_name ("PrimMatrixBoolCell");

   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);
   
   bind <WidgetMatrixBoolCell, &WidgetMatrixBoolCell::handle_event_default> (*this);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetMatrixBoolCell::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = opa::Rect (0, 0, 14, 14);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetMatrixBoolCell::do_get_margin (Margin & margin)
{
   margin = opa::Margin::_zero;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetMatrixBoolCell::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetMatrixBoolCell::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetMatrixBoolCell::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetMatrixBoolCell::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetMatrixBoolCell::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : req_gesture_entered
==============================================================================
*/

void  WidgetMatrixBoolCell::req_gesture_entered (opal::FrameSignal::Event & event)
{
   _prim_aptr->_hit_flag = true;
}



/*
==============================================================================
Name : req_gesture_exited
==============================================================================
*/

void  WidgetMatrixBoolCell::req_gesture_exited (opal::FrameSignal::Event & event)
{
   _prim_aptr->_hit_flag = false;
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void  WidgetMatrixBoolCell::req_gesture_signal (opal::FrameSignal::Event & event)
{
   Event widget_event (*this);
   widget_event._cur_state_flag = get_state ();
   
   widget_event._suggested_state_flag = !widget_event._cur_state_flag;
   
   _delegate.param_ref_signal (widget_event);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
