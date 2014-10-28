/*****************************************************************************

        WidgetSpin.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 68581

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

#include "ohm/opa/WidgetSpin.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opa/prim/PrimSpin.h"
#include "ohm/math/fnc.h"

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

WidgetSpin::~WidgetSpin ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_up_aptr.reset ();
      _frame_down_aptr.reset ();
   }
   catch (...)
   {
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

WidgetSpin::WidgetSpin ()
:  _abs_val (0)
{
   _frame_aptr = new opal::Frame ("WidgetSpin::frame");
   _frame_aptr->set_relative_origin (opa::Point (0, 0));
   _frame_aptr->set_bounds (opa::Size (9, 14));
   set_embeddable (*_frame_aptr);

   _frame_up_aptr = new opal::FrameSignal;
   _frame_up_aptr->set_bounds (opa::Size (9, 6));
   _frame_aptr->insert_top (*_frame_up_aptr);

   _frame_down_aptr = new opal::FrameSignal;
   _frame_down_aptr->set_origin (opa::Point (0, 8));
   _frame_down_aptr->set_bounds (opa::Size (9, 6));
   _frame_aptr->insert_top (*_frame_down_aptr);
   
   _frame_up_aptr->bind_rollover <
      WidgetSpin,
      &WidgetSpin::req_rollover_begin,
      &WidgetSpin::req_rollover_end
   > (*this);

   _frame_up_aptr->bind_gesture_trip <
      WidgetSpin,
      &WidgetSpin::req_gesture_entered,
      &WidgetSpin::req_gesture_exited
   > (*this);

   _frame_up_aptr->bind_gesture_signal <
      WidgetSpin, &WidgetSpin::req_gesture_signal
   > (*this);

   _frame_up_aptr->bind_gesture_timer <
      WidgetSpin, &WidgetSpin::req_gesture_timer
   > (*this);


   _frame_down_aptr->bind_rollover <
      WidgetSpin,
      &WidgetSpin::req_rollover_begin,
      &WidgetSpin::req_rollover_end
   > (*this);

   _frame_down_aptr->bind_gesture_trip <
      WidgetSpin,
      &WidgetSpin::req_gesture_entered,
      &WidgetSpin::req_gesture_exited
   > (*this);

   _frame_down_aptr->bind_gesture_signal <
      WidgetSpin, &WidgetSpin::req_gesture_signal
   > (*this);

   _frame_down_aptr->bind_gesture_timer <
      WidgetSpin, &WidgetSpin::req_gesture_timer
   > (*this);

   
   _prim_aptr = new PrimSpin;
   _prim_aptr->set_name ("PrimSpin");

   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetSpin::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = opa::Rect (0, -10, 9, 14);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetSpin::do_get_margin (Margin & margin)
{
   margin = opa::Margin (4, 2, 4, 3);
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetSpin::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetSpin::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void WidgetSpin::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetSpin::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetSpin::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: req_rollover_begin
==============================================================================
*/

void WidgetSpin::req_rollover_begin (opal::FrameSignal::Event & event)
{
   /*if (event._frame_ptr == &*_frame_up_aptr)
   {
      _prim_aptr->_state = PrimSpin::State_ROLLOVER_UP;
   }
   else if (event._frame_ptr == &*_frame_down_aptr)
   {
      _prim_aptr->_state = PrimSpin::State_ROLLOVER_DOWN;
   }*/
}



/*
==============================================================================
Name: req_rollover_end
==============================================================================
*/

void WidgetSpin::req_rollover_end (opal::FrameSignal::Event & event)
{
   /*_prim_aptr->_state = PrimSpin::State_NORMAL;*/
}



/*
==============================================================================
Name : req_gesture_entered
==============================================================================
*/

void  WidgetSpin::req_gesture_entered (opal::FrameSignal::Event & event)
{
   if (&event._frame == &*_frame_up_aptr)
   {
      _prim_aptr->_state = PrimSpin::State_ON_UP;
   }
   else if (&event._frame == &*_frame_down_aptr)
   {
      _prim_aptr->_state = PrimSpin::State_ON_DOWN;
   }
}



/*
==============================================================================
Name : req_gesture_exited
==============================================================================
*/

void  WidgetSpin::req_gesture_exited (opal::FrameSignal::Event & event)
{
   _prim_aptr->_state = PrimSpin::State_OFF;
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void  WidgetSpin::req_gesture_signal (opal::FrameSignal::Event & event)
{
   _abs_val = 0;
   int cur_abs_val = 1;
   int delta = cur_abs_val - _abs_val;
   _abs_val = cur_abs_val;
   
   signal (delta, event);
}



/*
==============================================================================
Name : req_gesture_timer
==============================================================================
*/

void  WidgetSpin::req_gesture_timer (opal::FrameSignal::Event & event)
{
   double dtime = event._time - event._gesture_start_time;
   assert (dtime >= 0.f);
   
   float val = float (dtime * dtime);
   
   int cur_abs_val = math::floor_int (val);
   int delta = cur_abs_val - _abs_val;
   _abs_val = cur_abs_val;
   
   signal (delta, event);
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  WidgetSpin::signal (int abs_delta, opal::FrameSignal::Event & event)
{
   if (abs_delta > 0)
   {
      Event widget_event (*this);
      
      if (&event._frame == &*_frame_up_aptr)
      {
         widget_event._delta = abs_delta;
      }
      else if (&event._frame == &*_frame_down_aptr)
      {
         widget_event._delta = - abs_delta;
      }
      
      _delegate.param_ref_signal (widget_event);
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

