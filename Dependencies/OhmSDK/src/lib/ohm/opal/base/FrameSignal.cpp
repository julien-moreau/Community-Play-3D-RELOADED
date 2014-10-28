/*****************************************************************************

        FrameSignal.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71263

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

#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/opal/Cursor.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*
==============================================================================
Contract Description :
   FrameSignal handles the user input event mediation of a signal or 2-state
   logic button.
   FrameSignal has 2 implicit states :
   - rollover
   - gesture
   each state is marked by a 'begin' and 'end' call. States are mutually
   exclusive : no gesture call can arise between a rollover_begin/end, and
   no rollover call can arise between a gesture_begin/end
   - rollover represents the state where the mouse left button is not pressed.
     rollover is optional
     - rollover_begin notifies when the user mouse location enters the frame
       bounds
     - rollover_end notifies when the user mouse location leaves the frame
       bounds
   - gesture represents the state where the mouse left button was pressed in
     the frame bounds
     - gesture_begin notifies about the beginning of the gesture
     - gesture_entered notifies when the mouse location enters the frame
       bounds
     - gesture_exited notifies when the mouse location leaves the frame bounds
     - gesture_signal notifies when the user confirmed the gesture (released
       withing frame bounds). The signal itself is an encoding of the
       logic state differential (none for a signal button, toggle for a
       2-state button)
     - gesture_timer_fired notifies regurarly while the gesture is ongoing.
       it may be or not orthogonal to entered/exited scheme.
       (depends on configuration)
       'dtime' represents the time interval since the gesture begun.
       timer is optional
     - gesture_end notifies about the end of the gesture
   
   Typical pattern
   Mouse is outside the frame,         |     Nothing
   Left button is not pressed          |

   Mouse comes inside the frame        |     req_rollover_begin

   Mouse goes outside the frame        |     req_rollover_end

   Mouse comes inside the frame again  |     req_rollover_begin

   Mouse left button is pressed        |     req_rollover_end
                                       |     then req_gesture_begin
                                       |     then req_gesture_entered
                                       |     then (optionally) req_gesture_signal

   Mouse goes outside the frame        |     req_gesture_exited

   Mouse comes inside the frame again  |     req_gesture_entered
   
   Mouse left button is released       |     (optionally) req_gesture_signal
                                       |     then req_gesture_exited
                                       |     then req_gesture_end
                                       |     then req_rollover_begin
   
   
==============================================================================
*/

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Event::ctor
==============================================================================
*/

FrameSignal::Event::Event (FrameSignal & frame)
:  _frame (frame)
,  _event_ptr (0)
,  _gesture_flag (false)
,  _in_flag (false)
,  _gesture_start_time (0.0)
,  _time (0.0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

FrameSignal::FrameSignal ()
:  opal::Frame ("FrameSignal")
,  _dispatcher (*this)
,  _event (*this)
,  _signal_on_pressed_flag (false)
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
,  _cursor_std (Application::StdCursor_ARROW)
#endif
{
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameSignal>::Button_LEFT,
      &FrameSignal::mouse_button_pressed,
      &FrameSignal::mouse_button_released,
      &FrameSignal::mouse_dragged
   );

   _dispatcher._mouse.configure_mouse_moved (
      &FrameSignal::mouse_dragged
   );

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   _dispatcher._mouse.configure_cursor (
      &FrameSignal::mouse_cursor
   );
#endif

   _dispatcher._user_input.configure_lost (
      &FrameSignal::user_input_lost
   );
   
   add (_dispatcher);
   
   bind_hit_test <FrameSignal, &FrameSignal::hit_test_default> (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameSignal::~FrameSignal ()
{
}

   
   
/*
==============================================================================
Name: set_signal_on_pressed
==============================================================================
*/

void  FrameSignal::set_signal_on_pressed (bool signal_on_pressed_flag)
{
   _signal_on_pressed_flag = signal_on_pressed_flag;
}



/*
==============================================================================
Name: is_signal_on_pressed
==============================================================================
*/

bool  FrameSignal::is_signal_on_pressed () const
{
   return _signal_on_pressed_flag;
}



/*
==============================================================================
Name: set_cursor_on_rollover
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  FrameSignal::set_cursor_on_rollover (const std::string & path, const opa::Point & hot_point)
{
   _cursor_path = path;
   _cursor_hot_point = hot_point;
}
#endif



/*
==============================================================================
Name: set_cursor_on_rollover
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  FrameSignal::set_cursor_on_rollover (const std::pair <std::string, opa::Point> & cursor)
{
   _cursor_path = cursor.first;
   _cursor_hot_point = cursor.second;
}
#endif



/*
==============================================================================
Name: set_cursor_on_rollover
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  FrameSignal::set_cursor_on_rollover (Application::StdCursor cursor)
{
   _cursor_path.clear ();
   _cursor_std = cursor;
}
#endif



/*
==============================================================================
Name : mouse_entered
==============================================================================
*/

void  FrameSignal::mouse_entered (opal::Event & event)
{
   if ((event._pass == opal::Event::Pass_SPATIAL) && (!_event._gesture_flag))
   {
      _event._previous_time = _event._time;
      _event._time = event._timer._time;
      _event._event_ptr = &event;
      mouse_handle (true);
   }
}



/*
==============================================================================
Name : mouse_exited
==============================================================================
*/

void  FrameSignal::mouse_exited (opal::Event & event)
{
   if ((event._pass == opal::Event::Pass_SPATIAL) && (!_event._gesture_flag))
   {
      _event._previous_time = _event._time;
      _event._time = event._timer._time;
      _event._event_ptr = &event;
      
      mouse_handle (false);
   }
}



/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FrameSignal::mouse_button_pressed (opal::Event & event)
{
   assert (!_event._gesture_flag);

   _event._previous_time = _event._time;
   _event._time = event._timer._time;
   _event._event_ptr = &event;
   
   bool old_in_flag = _event._in_flag;
   _hit_test.param_ref_signal (_event);
   bool in_flag = _event._in_flag;
   _event._in_flag = old_in_flag;
   
   if (in_flag)
   {
      acquire_mouse_capture ();
      
      mouse_handle (false);
      
      _event._gesture_flag = true;
      
      _event._gesture_start_time = event._timer._time;
      _event._previous_time = _event._time;
      
      _gesture_begin.param_ref_signal (_event);
      
      mouse_handle (true);
      
      if (_signal_on_pressed_flag)
      {
         _gesture_signal.param_ref_signal (_event);
      }
   
      event.block ();
   }
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  FrameSignal::mouse_button_released (opal::Event & event)
{
   if (_event._gesture_flag)
   {
      _event._event_ptr = &event;
      _event._previous_time = _event._time;
      _event._time = event._timer._time;
      
      release_mouse_capture ();
      
      bool old_in_flag = _event._in_flag;
      _hit_test.param_ref_signal (_event);
      bool in_flag = _event._in_flag;
      _event._in_flag = old_in_flag;
      
      if (in_flag && !_signal_on_pressed_flag)
      {
         _gesture_signal.param_ref_signal (_event);
      }
      
      mouse_handle (false);
      
      _gesture_end.param_ref_signal (_event);
      
      _event._gesture_flag = false;
      
      mouse_handle (in_flag);
      
      event.block ();
   }
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FrameSignal::mouse_dragged (opal::Event & event)
{
   if (_event._gesture_flag)
   {
      _event._event_ptr = &event;
      _event._previous_time = _event._time;
      _event._time = event._timer._time;
      
      bool old_in_flag = _event._in_flag;
      _hit_test.param_ref_signal (_event);
      bool in_flag = _event._in_flag;
      _event._in_flag = old_in_flag;
      
      mouse_handle (in_flag);
      
      event.block ();
   }
}



/*
==============================================================================
Name : mouse_cursor
==============================================================================
*/

void  FrameSignal::mouse_cursor (opal::Event & event)
{
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   if (_cursor_path.empty ())
   {
      Application::use_instance ().set (_cursor_std);
   }
   else
   {
      Application::use_instance ().set_cursor (_cursor_path, _cursor_hot_point);
   }

   event.block ();
#endif
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameSignal::user_input_lost (opal::Event & event)
{
   if (_event._gesture_flag)
   {
      _event._event_ptr = &event;
      _event._previous_time = _event._time;
      _event._time = event._timer._time;
      
      release_mouse_capture ();
      
      mouse_handle (false);
      
      _gesture_end.param_ref_signal (_event);

      _event._gesture_flag = false;
   }
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameSignal::timer_fired (opal::Event & event)
{
   assert (_gesture_timer.get_receiver_ptr () != 0);
   
   if (_event._gesture_flag)
   {
      _event._event_ptr = &event;
      _event._previous_time = _event._time;
      _event._time = event._timer._time;
      
      _gesture_timer.param_ref_signal (_event);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : mouse_handle
==============================================================================
*/

void  FrameSignal::mouse_handle (bool in_flag)
{
   bool old_in_flag = _event._in_flag;
   _event._in_flag = in_flag;
   
   if (_event._gesture_flag)
   {
      if (in_flag && !old_in_flag)
      {
         _gesture_entered.param_ref_signal (_event);
      }
      else if (old_in_flag && !in_flag)
      {
         _gesture_exited.param_ref_signal (_event);
      }
   }
   else
   {
      if (in_flag && !old_in_flag)
      {
         _rollover_begin.param_ref_signal (_event);
      }
      else if (old_in_flag && !in_flag)
      {
         _rollover_end.param_ref_signal (_event);
      }
   }
}



/*
==============================================================================
Name : hit_test_default
==============================================================================
*/

void  FrameSignal::hit_test_default (Event & event)
{
   opa::Rect rect (opa::Point::_zero, get_bounds ());
   
   event._in_flag = rect.contains (event._event_ptr->_mouse._location);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
