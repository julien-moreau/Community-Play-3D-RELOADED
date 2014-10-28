/*****************************************************************************

        WidgetCheckBox.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/opa/WidgetCheckBox.h"
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

WidgetCheckBox::~WidgetCheckBox ()
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

void  WidgetCheckBox::handle_event_default (Event & event)
{
   set_state (event._suggested_state);
}



/*
==============================================================================
Name : set_state
==============================================================================
*/

void  WidgetCheckBox::set_state (State state)
{
   _state = state;
   
   switch (_state)
   {
   case State_UNCHECKED:
      _prim_aptr->_state = PrimCheckBox::State_UNCHECKED;
      break;

   case State_CHECKED:
      _prim_aptr->_state = PrimCheckBox::State_CHECKED;
      break;

   case State_MIXED:
      _prim_aptr->_state = PrimCheckBox::State_MIXED;
      break;
   }
}



/*
==============================================================================
Name : get_state
==============================================================================
*/

WidgetCheckBox::State   WidgetCheckBox::get_state () const
{
   return _state;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetCheckBox::WidgetCheckBox ()
:  _frame_aptr ()
,  _prim_aptr ()
,  _state (State_UNCHECKED)
{
   _frame_aptr = new opal::FrameSignal;
   
   _frame_aptr->bind_gesture_trip <
      WidgetCheckBox,
      &WidgetCheckBox::req_gesture_entered,
      &WidgetCheckBox::req_gesture_exited
   > (*this);

   _frame_aptr->bind_gesture_signal <
      WidgetCheckBox, &WidgetCheckBox::req_gesture_signal
   > (*this);
   
   _frame_aptr->set_bounds (opa::Size (16, 16));
   
   _prim_aptr = new PrimCheckBox;
   _prim_aptr->set_name ("PrimCheckBox");

   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);
   
   bind <WidgetCheckBox, &WidgetCheckBox::handle_event_default> (*this);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetCheckBox::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = opa::Rect (0, -12, 16, 16);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetCheckBox::do_get_margin (Margin & margin)
{
   margin = opa::Margin (4, 2, 4, 3);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetCheckBox::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetCheckBox::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetCheckBox::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetCheckBox::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetCheckBox::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : req_gesture_entered
==============================================================================
*/

void  WidgetCheckBox::req_gesture_entered (opal::FrameSignal::Event & event)
{
   _prim_aptr->_hit_flag = true;
}



/*
==============================================================================
Name : req_gesture_exited
==============================================================================
*/

void  WidgetCheckBox::req_gesture_exited (opal::FrameSignal::Event & event)
{
   _prim_aptr->_hit_flag = false;
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void  WidgetCheckBox::req_gesture_signal (opal::FrameSignal::Event & event)
{
   Event widget_event (*this);
   widget_event._cur_state = get_state ();
   
   switch (widget_event._cur_state)
   {
   case State_UNCHECKED:
      widget_event._suggested_state = State_CHECKED;
      break;

   case State_CHECKED:
      widget_event._suggested_state = State_UNCHECKED;
      break;

   case State_MIXED:
      widget_event._suggested_state = State_CHECKED;
      break;
   }

   _delegate.param_ref_signal (widget_event);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
