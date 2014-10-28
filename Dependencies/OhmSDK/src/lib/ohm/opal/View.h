/*****************************************************************************

        View.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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



#if defined (ohm_opal_View_CURRENT_HEADER)
   #error Recursive inclusion of ohm/opal/View header.
#endif
#define  ohm_opal_View_CURRENT_HEADER

#if ! defined (ohm_opal_View_HEADER_INCLUDED)
#define  ohm_opal_View_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/ViewImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/carbon/ViewImpl.h"

#else
   #error Operation System not supported

#endif

#include "ohm/opal/Window.h"
#include "ohm/opal/StandardMouseCursor.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/Event.h"
#include "ohm/opal/HIDEvent.h"
#include "ohm/opal/FramePath.h"
#include "ohm/opal/private/TimerManager.h"
#include "ohm/opal/CaptureType.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/Observer.h"
#include "ohm/task/Timer.h"

#include <vector>
#include <string>
#include <set>

#define ohm_opal_VIEW_USE_EVENT_MGR

#if defined ohm_opal_VIEW_USE_EVENT_MGR
   #include "ohm/opal/private/SystemEventMgr.h"
#endif




namespace ohm
{
namespace opak
{
class Primitive;
}


namespace opal
{


class GraphicContextInterface;
class Primitive;

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   typedef ::HWND SystemView;

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   typedef HIViewRef SystemView;

#else
   #error Operation System not supported

#endif



class ConductChange;
class Cursor;
class Custom;
class Frame;
class Key;
class PostActionListener;
class TimeListener;

class View
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum State
   {
                  State_VOID = 0,
                  State_CTORED,
                  State_RUNNING,
   };
   
                  View (Window & window);
   virtual        ~View ();
   
   void           set_name (const char * name_0);
   
   // internal
   void           vt_init ();
   void           vt_restore ();
   void           vt_update (const opa::Size & size);
   void           vt_draw (bool wait_vbr_flag);
   
   SystemView     get_system_view ();
   Window &       use_window ();
   
   void           show ();
   void           hide ();

   void           track_window_layout (bool track_flag);
   void           set_origin (const opa::Point & point);
   void           set_bounds (const opa::Size & size);
   opa::Size      get_bounds ();
   
   void           bind (Primitive & primitive);
   void           unbind (Primitive & primitive);
   void           draw (GraphicContextInterface & context);
   void           invalidate (const opa::Rect & rect);
   
   Frame &        use_root_frame ();

   void           bind (Frame & frame);
   void           unbind (Frame & frame);
   void           notify_layout_changed ();
   void           process_layout_changed ();
   template <class T, void (T::*Method)()>
   void           bind_layout_notification (T & t);

   
   void           install_regular_timer (util::Delegate & delegate);
   void           install_one_shot_timer (util::Delegate & delegate, double delay);
   void           remove_timer (util::Delegate & delegate);
   void           remove_timers (Frame & frame);
   
   void           acquire_mouse_capture (Frame & frame);
   void           release_mouse_capture (Frame & frame);
   void           give_mouse_capture (Frame & frame);
   Frame *        get_mouse_capture_frame_ptr () const;
   
   void           acquire_keyboard_focus (Frame & frame);
   void           release_keyboard_focus (Frame & frame);
   void           give_keyboard_focus (Frame & frame);
   Frame *        get_keyboard_focus_frame_ptr () const;
   
   void           set_mouse_wheel_inversion (bool invert_flag);
   bool           is_mouse_wheel_inverted () const;
   
   void           post_conduct_changed_event (const ConductChange & conduct_change, Event::Direction direction);
   void           post (const Custom & custom, Event::Direction direction);
   void           post_update_cursor ();
   
   void           add (PostActionListener & listener);
   void           remove (PostActionListener & listener);

   void           add (TimeListener & listener);
   void           remove (TimeListener & listener);
   
   // Private use only
   ViewImpl &     use_impl ();
   const ViewImpl &
                  use_impl () const;

   void           debug_toggle_show_invalid_rect ();  

   void           post (const std::vector <HIDEvent> & hid_event_arr);


   
/*\\\ SYSTEM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
   const Event &  get_event () const;
   void           mouse_data_drag_set (const Data & data);
   void           user_input_lost ();
   void           window_layout_changed ();
   
#if defined ohm_opal_VIEW_USE_EVENT_MGR

   bool           handle_system_event (SystemEvent & event);

#endif // ohm_opal_VIEW_USE_EVENT_MGR

   friend class SystemEventMgr;

   void           mouse_button_pressed (int button_nbr, int click_count);
   void           mouse_button_released (int button_nbr);
   void           mouse_moved (const opa::Point & location, const opa::Point & delta);
   void           mouse_exited_view (const opa::Point & location, const opa::Point & delta);
   void           mouse_data_dropped (const opa::Point & location, const opa::Point & delta);
   void           mouse_wheel_moved (const opa::Point & delta);

   bool           key_pressed (const Key & key);
   bool           key_repeated (const Key & key);
   bool           key_released (const Key & key);
   void           key_cancelled ();
   bool           characters_typed (const std::string & str);
   void           keyboard_focus_lost ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
#if ! defined ohm_opal_VIEW_USE_EVENT_MGR
   friend class ViewImpl;
#endif
   
   typedef std::vector <Frame *> FramePtrList;
   typedef std::set <PostActionListener *> PostActionListenerPtrSet;
   typedef std::set <TimeListener *> TimeListenerPtrSet;
   
   enum PostponedEventType
   {
                  PostponedEventType_CONDUCT_CHANGED = 0,
                  // Event::Direction : direction top/bottom
                  
                  PostponedEventType_CUSTOM_SENT,
                  // Custom : custom
                  // Event::Direction : direction top/bottom

                  PostponedEventType_CURSOR_UPDATE,
                  // nothing
                  
                  PostponedEventType_GIVE_KEYBOARD_FOCUS,
                  // opal::Frame * : gifted frame

                  PostponedEventType_GIVE_MOUSE_CAPTURE,
                  // opal::Frame * : gifted frame
                  
                  PostponedEventType_LAYOUT_CHANGED,
                  // nothing

                  PostponedEventType_HID_EVENT_ARR,
                  // std::vector <HIDEvent>
   };
   
   void           timer_notify ();
   
   void           process (task::Event & event);
   void           process_conduct_changed (task::Event & event);
   void           process_custom_sent (task::Event & event);
   void           process_cursor_update (task::Event & event);
   void           process_give_mouse_capture (task::Event & event);
   void           process_give_keyboard_focus (task::Event & event);
   void           process_layout_changed (task::Event & event);
   void           process_hid_event_arr (task::Event & event);
   
   double         get_time ();
   
   void           pre_action ();
   void           post_action ();
   void           send_mouse_event ();
   void           send_mouse_cursor_event ();
   void           send_keyboard_event ();
   
   void           notify_mouse_target_lost ();
   
   void           view_layout_changed ();
   
   void           fake_mouse_moved ();
   void           send_debug ();
   void           update_time ();
   void           invalidate_time ();
   
   task::Observer
                  _observer;
   task::Timer    _timer;
   
   Primitive *    _primitive_ptr;
   
   Window &       _window;
   ViewImpl       _view_impl;
   sys::ThreadSync
                  _run_sync;
   sys::ThreadSync
                  _stop_sync;
   
   const char *   _name_0;
   
   Frame *        _root_frame_ptr;
   
   TimerManager   _timer_manager;
   
   bool           _processing_event_flag;
   Event          _event;
   
   FramePath      _mouse_event_target_path;
   FramePath      _keyboard_event_target_path;
   
   double         _time;
   bool           _time_valid_flag;
   
   TimeListenerPtrSet
                  _time_listener_ptr_set;
   PostActionListenerPtrSet
                  _post_action_listener_ptr_set;
   
   bool           _track_window_layout_flag;
   
   bool           _layout_changed_flag;
   
   util::Delegate _layout_delegate;
                  
#if defined ohm_opal_VIEW_USE_EVENT_MGR
   SystemEventMgr _sys_event_mgr;
#endif
   
   bool           _mouse_wheel_invert_flag;
   
   sys::Mutex     _debug_quad_arr_mtx;
   std::vector <opa::Quad>
                  _debug_quad_arr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  View ();
                  View (const View &other);
   View &         operator = (const View &other);
   bool           operator == (const View &other);
   bool           operator != (const View &other);

}; // class View



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/View.hpp"



#endif   // ohm_opal_View_HEADER_INCLUDED

#undef ohm_opal_View_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
