/*****************************************************************************

        View.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71208

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

#include "ohm/opal/View.h"
#include "ohm/opal/Cursor.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/opal/def.h"
#include "ohm/opal/Key.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/MenuBar.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/ConductChange.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/Primitive.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/opal/private/FrameBin.h"
#include "ohm/opal/private/PostActionListener.h"
#include "ohm/opal/private/TimeListener.h"
#include "ohm/opa/Path.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



#undef ohm_opal_VIEW_DEBUG_FRAME_FLAG



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Create a system view (HIView on MacOS X, Child Window with no enlightment
   on Windows) in to the window (WindowRef on MacOS X, Window handle
   on Windows) with 0,0 origin and window bounds.
   Initially the view is NOT visible.
==============================================================================
*/

View::View (Window & window)
:  _observer ("ohm.opal.View.postponed")
,  _timer ("ohm.opal.View.gui_timer")
,  _primitive_ptr (0)
,  _window (window)
,  _view_impl (window, *this)
,  _name_0 (0)
,  _root_frame_ptr (0)

,  _timer_manager ()

,  _processing_event_flag (false)
,  _event ()

,  _mouse_event_target_path ()
,  _keyboard_event_target_path ()

,  _time (0.0)
,  _time_valid_flag (false)

,  _track_window_layout_flag (true)
,  _layout_changed_flag (false)

#if defined ohm_opal_VIEW_USE_EVENT_MGR
,  _sys_event_mgr (*this)
#endif

,  _mouse_wheel_invert_flag (false)
{
   // will callback 'vt_init'
   VideoThread::use_instance ().add (*this);
   
   _run_sync.wait ();

   _root_frame_ptr = new Frame ("root");
   
   _root_frame_ptr->bind_root (*this);

   set_origin (opa::Point::_zero);
   
   window_layout_changed ();
   
   _view_impl.acquire_keyboard_focus ();
   _view_impl.post_view_layout_changed ();
   
   show ();

   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   
   // observer
   
   _observer.bind_process <
      View,
      &View::process
   > (*this);
   
   run_loop.add (_observer);
   
   // timer
   
   _timer.bind_process <
      View,
      &View::timer_notify
   > (*this);
   _timer.set_periodic (1.0 / 15.0);
   
   run_loop.add (_timer);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

View::~View ()
{
   try
   {
      task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   
      run_loop.remove (_observer);
      run_loop.remove (_timer);
      
      _root_frame_ptr->unbind_root ();
      
      // because of VC8 template class forward declaration problem
      // we mimic the AutoFramePtr class here
      
      AutoFramePtr <Frame>::take_out (_root_frame_ptr);
      
      FrameBin::use_instance ().recycle ();
      
      _name_0 = 0;

      // will callback 'vt_restore'
      VideoThread::use_instance ().remove (*this);
      
      _stop_sync.wait ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_name
==============================================================================
*/

void  View::set_name (const char * name_0)
{
   assert (_name_0 == 0);
   
   _name_0 = name_0;
}
   


/*
==============================================================================
Name : vt_init
Thread : Video
==============================================================================
*/

void  View::vt_init ()
{
   _view_impl.vt_init ();
   
   _run_sync.signal ();
}



/*
==============================================================================
Name : vt_restore
Thread : Video
==============================================================================
*/

void  View::vt_restore ()
{
   _view_impl.vt_restore ();
   
   _stop_sync.signal ();
}



/*
==============================================================================
Name : vt_update
Thread : Video
==============================================================================
*/

void  View::vt_update (const opa::Size & size)
{
   _view_impl.vt_update (size);
}



/*
==============================================================================
Name : vt_draw
Thread : Video
Description :
   wait for vertical retrace iff 'wait_vbr_flag' is true.
==============================================================================
*/

void  View::vt_draw (bool wait_vbr_flag)
{
   _view_impl.vt_draw (wait_vbr_flag);
}



/*
==============================================================================
Name : get_system_view
==============================================================================
*/

SystemView  View::get_system_view ()
{
   return _view_impl.get_system_view ();
}
   


/*
==============================================================================
Name : use_window
==============================================================================
*/

Window & View::use_window ()
{
   return _window;
}
   


/*
==============================================================================
Name : show
==============================================================================
*/

void  View::show ()
{
   _view_impl.show ();
}



/*
==============================================================================
Name : hide
==============================================================================
*/

void  View::hide ()
{
   _view_impl.hide ();
}



/*
==============================================================================
Name : track_window_layout
Description :
   if 'track_flag' is true, 'this' bounds will match parent window bounds
   changes. This should be only interesting when the parent window contains
   only one view.
   if 'track_flag' is false, 'this' bounds is unrelated to parent window
   bounds changes. This should be only interesting when the parent window
   contains more than on view.
==============================================================================
*/

void  View::track_window_layout (bool track_flag)
{
   _track_window_layout_flag = track_flag;
}



/*
==============================================================================
Name : set_origin
Description :
   'point' is in window relative coords
==============================================================================
*/

void  View::set_origin (const opa::Point & point)
{
   _view_impl.set_origin (point);
   
   VideoThread::use_instance ().update (*this);
   
   // view layout changed is not needed
   
   notify_layout_changed ();
}



/*
==============================================================================
Name : set_bounds
Description :
   Set the bounds of the view. This set the clipping rect, since the
   drawing in a view is always bounded to its bounds.
==============================================================================
*/

void  View::set_bounds (const opa::Size & size)
{
   _view_impl.set_bounds (size);
   
   VideoThread::use_instance ().update (*this);
   
   view_layout_changed ();
   
   notify_layout_changed ();
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

opa::Size   View::get_bounds ()
{
   return _view_impl.get_bounds ();
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  View::bind (Primitive & primitive)
{
   assert (&primitive != 0);
   
   _primitive_ptr = &primitive;
   
   //_view_impl.invalidate_all ();
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  View::unbind (Primitive & primitive)
{
   assert (&primitive == _primitive_ptr);
   
   _primitive_ptr = 0;
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  View::draw (GraphicContextInterface & context)
{
   if (_primitive_ptr != 0)
   {
      _primitive_ptr->draw (context);

#if defined (ohm_opal_VIEW_DEBUG_FRAME_FLAG)
      context.save_graphic_state ();
   
      std::vector <opa::Quad> debug_quad_arr;
   
      {
         sys::CritSec crit_sex (_debug_quad_arr_mtx);
         debug_quad_arr = _debug_quad_arr;
      }
      
      for (size_t i = 0 ; i < debug_quad_arr.size () ; ++i)
      {
         const opa::Quad & quad = debug_quad_arr [i];
         
         opa::Path path;
         path.move_to (quad [0]);
         path.line_to (quad [1]);
         path.line_to (quad [2]);
         path.line_to (quad [3]);
         path.line_to (quad [0]);
         
         context.set_stroke_color (opal::Color (1.f, 0.f, 1.f, 0.f));
         context.stroke (path);
      }
      
      context.restore_graphic_state ();
#endif
   }
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  View::invalidate (const opa::Rect & rect)
{
   opa::Rect final_rect = rect;

   opa::Rect clip_rect;
   clip_rect._size = get_bounds ();
   
   final_rect &= clip_rect;
   
   if (!final_rect.is_empty ())
   {
      final_rect.integral ();
   
      _view_impl.invalidate (final_rect);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  View::bind (Frame & frame)
{
   notify_layout_changed ();
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  View::unbind (Frame & frame)
{
   remove_timers (frame);

   if (!_mouse_event_target_path.empty ())
   {
      if (_mouse_event_target_path.back ().first == &frame)
      {
         release_mouse_capture (frame);
      }
   }
   
   if (!_keyboard_event_target_path.empty ())
   {
      if (_keyboard_event_target_path.back ().first == &frame)
      {
         release_keyboard_focus (frame);
      }
   }
   
   notify_layout_changed ();
}



/*
==============================================================================
Name : notify_layout_changed
==============================================================================
*/

void  View::notify_layout_changed ()
{
   _layout_changed_flag = true;
}



/*
==============================================================================
Name : process_layout_changed
==============================================================================
*/

void  View::process_layout_changed ()
{
   if (_layout_changed_flag)
   {
      _layout_changed_flag = false;
      
      task::Event & event = ohm_task_EVENT_GET;
      event.push (PostponedEventType_LAYOUT_CHANGED);
      _observer.send (event);
   }
}



/*
==============================================================================
Name : use_root_frame
==============================================================================
*/

Frame &  View::use_root_frame ()
{
   assert (_root_frame_ptr != 0);
   
   return *_root_frame_ptr;
}



/*
==============================================================================
Name : install_timer
Description :
==============================================================================
*/

void  View::install_regular_timer (util::Delegate & delegate)
{
   _timer_manager.install_regular_timer (delegate, get_time ());
}



/*
==============================================================================
Name : install_timer
Description :
==============================================================================
*/

void  View::install_one_shot_timer (util::Delegate & delegate, double delay)
{
   _timer_manager.install_one_shot_timer (delegate, delay, get_time ());
}



/*
==============================================================================
Name : remove_timer
Description :
==============================================================================
*/

void  View::remove_timer (util::Delegate & delegate)
{
   _timer_manager.remove_timer (delegate);
}



/*
==============================================================================
Name : remove_timers
Description :
==============================================================================
*/

void  View::remove_timers (Frame & frame)
{
   assert (&frame != 0);
   
   _timer_manager.remove_timers (&frame);
}



/*
==============================================================================
Name : acquire_mouse_capture
Description :
==============================================================================
*/

void  View::acquire_mouse_capture (Frame & frame)
{
   assert (&frame != 0);
   
   // the mouse event target and its path (frame chain) is contained in the
   // current event
   
   assert (!_event.get_frame_path ().empty ());
   assert (_event.get_frame_path ().back ().first == &frame);
   
   notify_mouse_target_lost ();
   
   _mouse_event_target_path = _event.get_frame_path ();
}



/*
==============================================================================
Name : release_mouse_capture
==============================================================================
*/

void  View::release_mouse_capture (Frame & frame)
{
   assert (&frame != 0);
   
   assert (!_mouse_event_target_path.empty ());
   assert (_mouse_event_target_path.back ().first == &frame);

   notify_mouse_target_lost ();
}



/*
==============================================================================
Name : give_mouse_capture
Description :
   Give mouse capture to 'frame'
Note :
   This is to be used when the frame in need of mouse capture cannot acquire
   the capture because it will not process the event that originates this need.
   This will send an event to avoid reentrancy.
   The current captured frame (if any) will automatically lose mouse capture.
   The gifted frame *must* acquire the capture to "accept the mouse capture
   gift".
==============================================================================
*/

void  View::give_mouse_capture (Frame & frame)
{
   assert (&frame != 0);
   
   // make current frame (if any) lose the focus
   
   if (!_mouse_event_target_path.empty ())
   {
      release_mouse_capture (*_mouse_event_target_path.back ().first);
   }
   
   // send the event
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (PostponedEventType_GIVE_MOUSE_CAPTURE);
   event.push (&frame);
   _observer.send (event);
}



/*
==============================================================================
Name : get_mouse_capture_frame_ptr
Description :
==============================================================================
*/

Frame *  View::get_mouse_capture_frame_ptr () const
{
   // may be null
   
   if (_mouse_event_target_path.empty ())
   {
      return 0;
   }
   else
   {
      return _mouse_event_target_path.back ().first;
   }
}



/*
==============================================================================
Name : acquire_keyboard_focus
Description :
   Set the focus target for this view, and ask the system from the view
   system implementation to acquire keyboard focus.
   We let the view implementation to handle focus to allow perfectly
   compliant code for systems that support keyboard focus natively.
==============================================================================
*/

void  View::acquire_keyboard_focus (Frame & frame)
{
   assert (&frame != 0);
   
   // the event target and its path is contained in the
   // current event
   
   assert (!_event.get_frame_path ().empty ());
   assert (_event.get_frame_path ().back ().first == &frame);
   
   if (!_keyboard_event_target_path.empty ())
   {
      Frame * sub_frame_ptr = _keyboard_event_target_path.back ().first;
      assert (sub_frame_ptr != 0);
      
      release_keyboard_focus (*sub_frame_ptr);
   }
   
   _keyboard_event_target_path = _event.get_frame_path ();
   
   _view_impl.acquire_keyboard_focus ();
}



/*
==============================================================================
Name : release_keyboard_focus
Description :
   Release the focus target for this view, and ask the system from the view
   system implementation to release keyboard focus.
   We let the view implementation to handle focus to allow perfectly
   compliant code for systems that support keyboard focus natively.
==============================================================================
*/

void  View::release_keyboard_focus (Frame & frame)
{
   assert (&frame != 0);
   
   if (!_keyboard_event_target_path.empty ())
   {
      Frame * frame_ptr = _keyboard_event_target_path.back ().first;
      assert (frame_ptr == &frame);
      
      // clear before calling 'keyboard_focus_lost'
      // so that 'get_keyboard_focus_frame_ptr' returns 0.
      _keyboard_event_target_path.clear ();

      frame_ptr->keyboard_focus_lost (_event);
   }
}



/*
==============================================================================
Name : give_keyboard_focus
Description :
   Give the focus to 'frame'
Note :
   This is to be used when the frame in need of keyboard focus cannot acquire
   the focus because it will not process the event that originates this need.
   This will send an event to avoid reentrancy.
   The current focused frame (if any) will automatically lose keyboard focus.
   The gifted frame *must* acquire the focus to "accept the keyboard
   focus gift".
==============================================================================
*/

void  View::give_keyboard_focus (Frame & frame)
{
   assert (&frame != 0);
   
   // make current frame (if any) lose the focus
   
   if (!_keyboard_event_target_path.empty ())
   {
      release_keyboard_focus (*_keyboard_event_target_path.back ().first);
   }
   
   // send the event
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (PostponedEventType_GIVE_KEYBOARD_FOCUS);
   event.push (&frame);
   _observer.send (event);
}



/*
==============================================================================
Name : get_keyboard_focus_frame_ptr
Description :
==============================================================================
*/

Frame *  View::get_keyboard_focus_frame_ptr () const
{
   // may be null
   
   if (_keyboard_event_target_path.empty ())
   {
      return 0;
   }
   else
   {
      return _keyboard_event_target_path.back ().first;
   }
}



/*
==============================================================================
Name: set_mouse_wheel_inversion
==============================================================================
*/

void  View::set_mouse_wheel_inversion (bool invert_flag)
{
   _mouse_wheel_invert_flag = invert_flag;
}



/*
==============================================================================
Name: is_mouse_wheel_inverted
==============================================================================
*/

bool  View::is_mouse_wheel_inverted () const
{
   return _mouse_wheel_invert_flag;
}



/*
==============================================================================
Name: post_conduct_changed_event
==============================================================================
*/

void View::post_conduct_changed_event (const ConductChange & conduct_change, Event::Direction direction)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (PostponedEventType_CONDUCT_CHANGED);
   event.push (direction);
   event.push (conduct_change);

   _observer.send (event);
}



/*
==============================================================================
Name : post
==============================================================================
*/

void  View::post (const Custom & custom, Event::Direction direction)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (PostponedEventType_CUSTOM_SENT);
   event.push (custom);
   event.push (direction);

   _observer.send (event);
}



/*
==============================================================================
Name : post_update_cursor
==============================================================================
*/

void  View::post_update_cursor ()
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (PostponedEventType_CURSOR_UPDATE);

   _observer.send (event);
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  View::add (PostActionListener & listener)
{
   assert (&listener != 0);
   
   _post_action_listener_ptr_set.insert (&listener);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  View::remove (PostActionListener & listener)
{
   assert (&listener != 0);
   
   _post_action_listener_ptr_set.erase (&listener);
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  View::add (TimeListener & listener)
{
   assert (&listener != 0);
   
   _time_listener_ptr_set.insert (&listener);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  View::remove (TimeListener & listener)
{
   assert (&listener != 0);
   
   _time_listener_ptr_set.erase (&listener);
}



/*
==============================================================================
Name : mouse_button_pressed
Description :
   Run the mhc list for mouse button pressed msg.
   Since the view is opaque to clicks, there is no need to return a reply,
   which is equivalent to say that the message was catched.
==============================================================================
*/

void  View::mouse_button_pressed (int button_nbr, int click_count)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   // double click distance is 3 pixels
   
   assert (click_count > 0);
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_MOUSE_BUTTON_PRESSED;
   _event._mouse._pressed_flag = true;
   _event._mouse._button_nbr = button_nbr;
   _event._mouse._click_count = click_count;
   
   if (_event._mouse._click_count == 1)
   {
      _event._mouse._view_relative_click_location = _event._mouse._view_relative_location;
      _event._mouse._click_location.invalidate ();
   }
   
   //assert (_event._data_ptr == 0);
   assert (_event._str == "");
   
   // send
   
   send_mouse_event ();
   
   post_action ();
}



/*
==============================================================================
Name : mouse_button_released
Description :
   Run the mhc list for mouse button pressed msg.
   Since the view is opaque to clicks, there is no need to return a reply,
   which is equivalent to say that the message was catched.
==============================================================================
*/

void  View::mouse_button_released (int button_nbr)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_MOUSE_BUTTON_RELEASED;
   _event._mouse._pressed_flag = false;
   _event._mouse._button_nbr = button_nbr;
   
   assert (_event._str == "");
   
   // send
   
   send_mouse_event ();
   
   
   post_action ();
}



/*
==============================================================================
Name : mouse_moved
Description :
   'delta' may be 0,0 if there is no way to know it
   Each time the mouse move, this msg can be sent to the view.
   It can be even outside the view when it is propagated (even if no
   capture is ongoing)
   This msg will propagate all needed mouse enter/leave normal/drag msgs.
   an additionnal mouse_moved must be called at :
   - after each time a frame is inserted in the frame tree (done in
     rebuild_msg_list to ensure it is after a msg propagation)
   - after each end of mouse capture
   - when the view is created by polling the mouse
   - after each time the view layout is changed
   - after each time the frame tree layout changes
   - after each time a mhc is put in a frame
   mouse movement messages cannot be catched
==============================================================================
*/

void  View::mouse_moved (const opa::Point & location, const opa::Point & delta)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_MOUSE_MOVED;
   _event._mouse._view_relative_location = location;
   _event._mouse._location.invalidate ();
   
   _event._mouse._view_relative_location_delta = delta;
   _event._mouse._location_delta.invalidate ();
   
   assert (_event._str == "");
   
   // send
   
   send_mouse_event ();
   send_mouse_cursor_event ();
   
   post_action ();
}



/*
==============================================================================
Name : mouse_exited_view
Description :
==============================================================================
*/

void  View::mouse_exited_view (const opa::Point & location, const opa::Point & delta)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_MOUSE_MOVED;
   _event._mouse._view_relative_location = location;
   _event._mouse._location.invalidate ();
   
   _event._mouse._view_relative_location_delta = delta;
   _event._mouse._location_delta.invalidate ();
   _event._mouse_in_flag = false;
   
   assert (_event._str == "");
   
   // start in blocked mode
   //_event.block ();
   
   // send
   
   send_mouse_event ();
   
   post_action ();
}



/*
==============================================================================
Name : mouse_data_drag_set
Description :
   Attach data to the event, but does not propagate an event itself.
   (it needs to be followed by a mouse event)
   The data itself is cleared after the next received event.
==============================================================================
*/

void  View::mouse_data_drag_set (const Data & data)
{
   _event._data = data;
}



/*
==============================================================================
Name : mouse_data_dropped
==============================================================================
*/

void  View::mouse_data_dropped (const opa::Point & location, const opa::Point & delta)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_DATA_DROPPED;
   _event._mouse._view_relative_location = location;
   _event._mouse._location.invalidate ();
   
   _event._mouse._view_relative_location_delta = delta;
   _event._mouse._location_delta.invalidate ();
   
   assert (_event._str == "");
   
   // send
   
   send_mouse_event ();
   
   post_action ();
}



/*
==============================================================================
Name : mouse_wheel_moved
Description :
==============================================================================
*/

void  View::mouse_wheel_moved (const opa::Point & delta)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_MOUSE_WHEEL_MOVED;
   if (_mouse_wheel_invert_flag)
   {
      _event._mouse._view_relative_wheel_delta._x = delta._x;
      _event._mouse._view_relative_wheel_delta._y = - delta._y;
   }
   else
   {
      _event._mouse._view_relative_wheel_delta = delta;
   }
   _event._mouse._wheel_delta.invalidate ();
   
   assert (_event._str == "");
   
   // send
   
   send_mouse_event ();
   
   post_action ();
}



/*
==============================================================================
Name : key_pressed
Description :
   This msg is sent from the application (through each window, then view)
   when the target of the focus did not catch the key.
   The application first call its keystroke manager with the key,
   then call the active window (and each view into it)
   then call all flowting windows (and each view into it)
   it does NOT call non active document window
==============================================================================
*/

bool  View::key_pressed (const Key & key)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_KEY_PRESSED;
   _event._key = key;
   
   // send
   
   send_keyboard_event ();
   
   post_action ();

   return _event.is_blocked ();
}



/*
==============================================================================
Name : key_repeated
Description :
   This msg is sent from the application (through each window, then view)
   when the target of the focus did not catch the key.
   The application first call its keystroke manager with the key,
   then call the active window (and each view into it)
   then call all flowting windows (and each view into it)
   it does NOT call non active document window
==============================================================================
*/

bool  View::key_repeated (const Key & key)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_KEY_REPEATED;
   _event._key = key;
   
   // send
   
   send_keyboard_event ();
   
   post_action ();

   return _event.is_blocked ();
}



/*
==============================================================================
Name : key_released
Description :
   This msg is sent from the application (through each window, then view)
   when the target of the focus did not catch the key.
   The application first call its keystroke manager with the key,
   then call the active window (and each view into it)
   then call all flowting windows (and each view into it)
   it does NOT call non active document window
==============================================================================
*/

bool  View::key_released (const Key & key)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_KEY_RELEASED;
   _event._key = key;
   
   // send
   
   send_keyboard_event ();
   
   post_action ();

   return _event.is_blocked ();
}



/*
==============================================================================
Name : characters_typed
Description :
==============================================================================
*/

bool  View::characters_typed (const std::string & str)
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   // prepare internal event
   
   _event._type = Event::Type_CHARACTERS_TYPED;
   _event._str = str;
   
   // send
   
   send_keyboard_event ();
   
   post_action ();
   
   _event._str = "";

   return _event.is_blocked ();
}



/*
==============================================================================
Name : keyboard_focus_lost
Description :
==============================================================================
*/

void  View::keyboard_focus_lost ()
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   if (! _processing_event_flag)
   {
      pre_action ();
      
      assert (_event.get_frame_path ().size () == 0);
      assert (_event.get_state_stack_size () == 1);

      if (!_keyboard_event_target_path.empty ())
      {
         release_keyboard_focus (*_keyboard_event_target_path.back ().first);
      }
      
      assert (_event.get_frame_path ().size () == 0);
      assert (_event.get_state_stack_size () == 1);

      post_action ();
   }
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  View::user_input_lost ()
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   assert (_root_frame_ptr != 0);
   
   pre_action ();
   
   // update internal event
   
   _event._type = Event::Type_USER_INPUT_LOST;
   _event._mouse._pressed_flag = false;
   _event._mouse._button_nbr = 0;
   _event._mouse._click_count = 0;
   
   assert (_event._str == "");
   
   // propagate
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   _event.push (*_root_frame_ptr);
   
   _root_frame_ptr->handle_event (_event);
   
   _event.pop ();
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   post_action ();
}



/*
==============================================================================
Name : timer_notify
Description :
   This is called regularly from the view.
==============================================================================
*/

void  View::timer_notify ()
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   pre_action ();
   
   _timer_manager.tick (_event);
   
   post_action ();

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG) 
   Application::use_instance ().apply_cursor ();
#endif
}



/*
==============================================================================
Name : window_layout_changed
Description :
   This message is sent to the view when the window bounds changes.
   The default implementation will look if the bounds of the view are
   synced to the windows, and change the bounds of the view to match
   the ones of the window.
   If a more complicated view layout is needed, this function must be
   derived, and the function set_origin and set_bounds called as needed.
==============================================================================
*/

void  View::window_layout_changed ()
{
   if (_track_window_layout_flag)
   {
      opa::Size bounds = _window.get_bounds ();
         
      set_bounds (bounds);
   }
}



/*
==============================================================================
Name : get_event
==============================================================================
*/

const Event &  View::get_event () const
{
   return _event;
}



/*
==============================================================================
Name : use_impl
==============================================================================
*/

ViewImpl &  View::use_impl ()
{
   return (_view_impl);
}



/*
==============================================================================
Name : use_impl
==============================================================================
*/

const ViewImpl &  View::use_impl () const
{
   return (_view_impl);
}



/*
==============================================================================
Name : debug_toggle_show_invalid_rect
==============================================================================
*/

void  View::debug_toggle_show_invalid_rect ()
{
   _view_impl.debug_toggle_show_invalid_rect ();
}



/*
==============================================================================
Name : post
Note :
   we might need to use 'handle_system_event' system but it is badly designed
   for now.
==============================================================================
*/

void  View::post (const std::vector <HIDEvent> & hid_event_arr)
{
   task::Event & event = ohm_task_EVENT_GET;

   event.push (PostponedEventType_HID_EVENT_ARR);
   event.push (hid_event_arr);

   _observer.send (event);
}



/*
==============================================================================
Name: handle_system_event
==============================================================================
*/

#if defined ohm_opal_VIEW_USE_EVENT_MGR
bool View::handle_system_event (SystemEvent & event)
{
   return _sys_event_mgr.process (event);
}
#endif


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process
Description :
==============================================================================
*/

void  View::process (task::Event & event)
{
   pre_action ();
   
   PostponedEventType type = event.pop <PostponedEventType> ();
   
   switch (type)
   {
   case PostponedEventType_CONDUCT_CHANGED:
      process_conduct_changed (event);
      break;

   case PostponedEventType_CUSTOM_SENT:
      process_custom_sent (event);
      break;

   case PostponedEventType_CURSOR_UPDATE:
      process_cursor_update (event);
      break;

   case PostponedEventType_GIVE_MOUSE_CAPTURE:
      process_give_mouse_capture (event);
      break;
   
   case PostponedEventType_GIVE_KEYBOARD_FOCUS:
      process_give_keyboard_focus (event);
      break;
   
   case PostponedEventType_LAYOUT_CHANGED:
      process_layout_changed (event);
      break;
   
   case PostponedEventType_HID_EVENT_ARR:
      process_hid_event_arr (event);
      break;
   
   default:
      assert (false);
      break;
   }
   
   post_action ();
}



/*
==============================================================================
Name : process_conduct_changed
==============================================================================
*/

void  View::process_conduct_changed (task::Event & event)
{
   assert (_root_frame_ptr != 0);
   
   _event._type = Event::Type_CONDUCT_CHANGED;
   _event._direction = event.pop <Event::Direction> ();
   _event._conduct_change = event.pop <ConductChange> ();
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // propagate to the whole tree with spacialisation
   
   _event._pass = Event::Pass_SPATIAL;
   
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // then, if not catched, propagate to the whole tree without spacialisation
   
   _event._pass = Event::Pass_CRUMB;
   
   if ((!_event.is_blocked ()) && (!_event.are_passes_skipped ()))
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   _event._conduct_change.reset ();
}



/*
==============================================================================
Name : process_custom_sent
==============================================================================
*/

void  View::process_custom_sent (task::Event & event)
{
   assert (_root_frame_ptr != 0);
   
   _event._type = Event::Type_CUSTOM_SENT;
   _event._custom.copy (event.get <Custom> ());
   event.skip <Custom> ();
   _event._direction = event.pop <Event::Direction> ();
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // propagate to the whole tree with spacialisation
   
   _event._pass = Event::Pass_SPATIAL;
   
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // then, if not catched, propagate to the whole tree without spacialisation
   
   _event._pass = Event::Pass_CRUMB;
   
   if ((!_event.is_blocked ()) && (!_event.are_passes_skipped ()))
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   _event._custom.reset ();
}



/*
==============================================================================
Name : process_cursor_update
==============================================================================
*/

void  View::process_cursor_update (task::Event & event)
{
   send_mouse_cursor_event ();
}



/*
==============================================================================
Name : process_give_mouse_capture
==============================================================================
*/

void  View::process_give_mouse_capture (task::Event & event)
{
   _event._type = Event::Type_MOUSE_CATCH_CAPTURE;
   _event._pass = Event::Pass_CRUMB;
   _event._mouse_capture_gifted_frame_ptr = event.pop <Frame *> ();
   assert (_event._mouse_capture_gifted_frame_ptr != 0);
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   _event.push (*_root_frame_ptr);
   _root_frame_ptr->handle_event (_event);
   _event.pop ();
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
}



/*
==============================================================================
Name : process_give_keyboard_focus
==============================================================================
*/

void  View::process_give_keyboard_focus (task::Event & event)
{
   _event._type = Event::Type_KEYBOARD_CATCH_FOCUS;
   _event._pass = Event::Pass_CRUMB;
   _event._keyboard_catch_focus._type = KeyboardCatchFocus::Type_GIFT;
   _event._keyboard_catch_focus._gifted_frame_ptr = event.pop <Frame *> ();
   assert (_event._keyboard_catch_focus._gifted_frame_ptr != 0);
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   _event.push (*_root_frame_ptr);
   _root_frame_ptr->handle_event (_event);
   _event.pop ();
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
}



/*
==============================================================================
Name : process_layout_changed
==============================================================================
*/

void  View::process_layout_changed (task::Event & event)
{
   // pre/post_action is goind to get called from within 'fake_mouse_moved'
   // (since it calls mouse_moved) so we release the action and take
   // it just after
   
   post_action ();
   
   fake_mouse_moved ();
   
   pre_action ();

#if defined (ohm_opal_VIEW_DEBUG_FRAME_FLAG) 
   send_debug ();
#endif
}



/*
==============================================================================
Name : post
Note :
   we might need to use 'handle_system_event' system but it is badly designed
   for now.
==============================================================================
*/

void  View::process_hid_event_arr (task::Event & event)
{
   std::vector <HIDEvent> hid_event_arr = event.pop <std::vector <HIDEvent> > ();
   
   for (size_t i = 0 ; i < hid_event_arr.size () ; ++i)
   {
      const HIDEvent & hid_event = hid_event_arr [i];
      
      // hid_event.trace ();
      
      // first try menubar
      
      if (hid_event._type == HIDEvent::Type_KEY_PRESSED)
      {
         bool catched_flag = MenuBar::use_instance ().post (hid_event._key);
         
         if (catched_flag)
         {
            break;
         }
      }
      
      // then the view
      
      switch (hid_event._type)
      {
      case HIDEvent::Type_KEY_PRESSED:
         _event._type = Event::Type_KEY_PRESSED;
         _event._key = hid_event._key;
         send_keyboard_event ();
         break;
      
      case HIDEvent::Type_KEY_REPEATED:
         _event._type = Event::Type_KEY_REPEATED;
         _event._key = hid_event._key;
         send_keyboard_event ();
         break;
      
      case HIDEvent::Type_KEY_RELEASED:
         _event._type = Event::Type_KEY_RELEASED;
         _event._key = hid_event._key;
         send_keyboard_event ();
         break;
      
      case HIDEvent::Type_CHARACTERS_TYPED:
         _event._type = Event::Type_CHARACTERS_TYPED;
         _event._str = hid_event._str;
         send_keyboard_event ();
         _event._str = "";
         break;
      
      default:
         assert (false);
         break;
      }
      
      if (_event.is_blocked ())
      {
         break;
      }
   }
}



/*
==============================================================================
Name : get_time
Description :
==============================================================================
*/

double   View::get_time ()
{
   double ret_val = 0.0;
   
   if (_time_valid_flag)
   {
      ret_val = _time;
   }
   else
   {
      // this situation arises when the time is requested outside
      // of a GUI msg. In that case we return the current time
      // instead of the cached one, which might be a lot innacurate
      
      ret_val = sys::TimerReliable::use_instance ().get ();
   }
   
   return ret_val;
}



/*
==============================================================================
Name : pre_action
Description :
   Update the current event :
   - the mouse is persistent
   - clear key
   - clear string
   - update time
   - data is persistent
   
==============================================================================
*/

void  View::pre_action ()
{
   _processing_event_flag = true;
   
   {
      PostActionListenerPtrSet::iterator it = _post_action_listener_ptr_set.begin ();
      PostActionListenerPtrSet::iterator it_end = _post_action_listener_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         PostActionListener * listener_ptr = *it;
         assert (listener_ptr != 0);
         
         listener_ptr->notify_pre_action ();
      }
   }
   
   _event.reset ();
   
   update_time ();
   
   _event._timer._time = get_time ();
   _event._timer._start_time = get_time ();
   
   {
      TimeListenerPtrSet::iterator it = _time_listener_ptr_set.begin ();
      TimeListenerPtrSet::iterator it_end = _time_listener_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         TimeListener * listener_ptr = *it;
         assert (listener_ptr != 0);
         
         listener_ptr->notify_time (get_time ());
      }
   }
   
   // don't touch _event._data_ptr
}



/*
==============================================================================
Name : post_action
==============================================================================
*/

void  View::post_action ()
{
   assert (_root_frame_ptr != 0);

   _processing_event_flag = false;
   
   FrameBin::use_instance ().recycle ();
   
   process_layout_changed ();
   
   invalidate_time ();
   
   {
      PostActionListenerPtrSet::iterator it = _post_action_listener_ptr_set.begin ();
      PostActionListenerPtrSet::iterator it_end = _post_action_listener_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         PostActionListener * listener_ptr = *it;
         assert (listener_ptr != 0);
         
         listener_ptr->notify_post_action ();
      }
   }
   
   // clear data
   _event._data._mime_type_data_map_arr.clear ();
}



/*
==============================================================================
Name : send_mouse_event
==============================================================================
*/

void  View::send_mouse_event ()
{
   assert (_root_frame_ptr != 0);
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   // first propagate to capture frame
   
   _event._pass = Event::Pass_FOCUS;
   
   _event.debug_print ();
   
   if (!_mouse_event_target_path.empty ())
   {
      _event._target_frame_path = _mouse_event_target_path;
      
      _event.push (*_root_frame_ptr);
   
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
      
      _event._target_frame_path.clear ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   // then, if not catched, propagate to the whole tree with spacialisation
   
   _event._pass = Event::Pass_SPATIAL;
   
   _event.debug_print ();
   
   if ((!_event.is_blocked ()) && (!_event.are_passes_skipped ()))
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // then, if not catched, propagate to the whole tree without spacialisation
   
   _event._pass = Event::Pass_CRUMB;
   
   _event.debug_print ();
   
   if ((!_event.is_blocked ()) && (!_event.are_passes_skipped ()))
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
}



/*
==============================================================================
Name : send_mouse_cursor_event
==============================================================================
*/

void  View::send_mouse_cursor_event ()
{
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   if (!_mouse_event_target_path.empty ())
   {
      return;
   }
   
   assert (_root_frame_ptr != 0);
   
   _event.reset ();
   _event._type = Event::Type_MOUSE_CURSOR;
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // propagate to the whole tree with spacialisation
   
   _event._pass = Event::Pass_SPATIAL;
   
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   // then, if not catched, set the default cursor
   
   if (!_event.is_blocked ())
   {
      Application::use_instance ().set_cursor_arrow ();
   }
#endif
}



/*
==============================================================================
Name : send_keyboard_event
==============================================================================
*/

void  View::send_keyboard_event ()
{
   assert (_root_frame_ptr != 0);
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   // first propagate to focus frame
   
   _event._pass = Event::Pass_FOCUS;
   
   if (!_keyboard_event_target_path.empty ())
   {
      _event._target_frame_path = _keyboard_event_target_path;
      
      _event.push (*_root_frame_ptr);
   
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
      
      _event._target_frame_path.clear ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // then, if not catched, propagate to the whole tree with spacialisation
   
   _event._pass = Event::Pass_SPATIAL;
   
   if ((!_event.is_blocked ()) && (!_event.are_passes_skipped ()))
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);

   // then, if not catched, propagate to the whole tree without spacialisation
   
   _event._pass = Event::Pass_CRUMB;
   
   if ((!_event.is_blocked ()) && (!_event.are_passes_skipped ()))
   {
      _event.push (*_root_frame_ptr);
      
      _root_frame_ptr->handle_event (_event);
      
      _event.pop ();
   }
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
}



/*
==============================================================================
Name : notify_mouse_target_lost
==============================================================================
*/

void  View::notify_mouse_target_lost ()
{
   if (!_mouse_event_target_path.empty ())
   {
      Frame * frame_ptr = _mouse_event_target_path.back ().first;
      
      // clear before calling 'mouse_target_lost'
      // so that 'get_mouse_capture_frame_ptr' return 0
      _mouse_event_target_path.clear ();

      frame_ptr->mouse_target_lost (_event);
   }
}



/*
==============================================================================
Name : view_layout_changed
Description :
   Set the root bounds to the view bounds.
==============================================================================
*/

void  View::view_layout_changed ()
{
   assert (_root_frame_ptr != 0);
   
   opa::Size bounds = _view_impl.get_bounds ();
   
   _root_frame_ptr->set_bounds (bounds);

   _layout_delegate.signal ();
}



/*
==============================================================================
Name : fake_mouse_moved
Description :
   propagate a fake mouse moved, only if it is in the view and no capture
   is under going.
==============================================================================
*/

void  View::fake_mouse_moved ()
{
   if (!_event._mouse._pressed_flag)
   {
      mouse_moved (_event._mouse._view_relative_location, opa::Point::_zero);
   }
}



/*
==============================================================================
Name : send_debug
==============================================================================
*/

void  View::send_debug ()
{
   assert (task::RunLoopGui::use_instance ().is_processing_event ());
   
   assert (_root_frame_ptr != 0);
   
   // update internal event
   
   _event._type = Event::Type_DEBUG;
   _event._debug_quad_arr.clear ();
   
   // propagate
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   _event.push (*_root_frame_ptr);
   
   _root_frame_ptr->handle_event (_event);
   
   _event.pop ();
   
   assert (_event.get_frame_path ().size () == 0);
   assert (_event.get_state_stack_size () == 1);
   
   // display
   
   {
      sys::CritSec crit_sec (_debug_quad_arr_mtx);
      
      _debug_quad_arr = _event._debug_quad_arr;
   }
}



/*
==============================================================================
Name : update_time
Description :
==============================================================================
*/

void  View::update_time ()
{
   _time = sys::TimerReliable::use_instance ().get ();
   _time_valid_flag = true;
}



/*
==============================================================================
Name : invalidate_time
Description :
   This will force a subsequent get_time call to get the real time instead
   of the cached one.
   This is needed when a msg comes from a non-GUI msg (for example a network
   callback) to ensure that collected time is right.
==============================================================================
*/

void  View::invalidate_time ()
{
   _time_valid_flag = false;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
