/*****************************************************************************

        ViewImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71392

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

#include "ohm/opal/carbon/ViewImpl.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/View.h"
#include "ohm/opal/carbon/ApplicationImpl.h"
#include "ohm/opal/carbon/EventFnc.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/def.h"
#include "ohm/opal/opengl/OpenGLEngine.h"
#include "ohm/opal/LabelString.h"
#include "ohm/util/trace.h"
#include "ohm/sys/TimerFine.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/carbon/RunLoopGuiImpl.h"
#include "ohm/sys/FileRefFactory.h"

#include <glut/glut.h>
#include <cassert>

#undef OHM_OPAL_METHOD_LATENT_SHAPE_INVALIDATE
#define OHM_OPAL_METHOD_IMMEDIATE_RECT_INVALIDATE



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Creates a HIView with dummy rects, and keep it hidded.
==============================================================================
*/

ViewImpl::ViewImpl (Window & window, View & view)
:  _window (window)
,  _view (view)
,  _hi_view_ref (0)
,  _hi_view_event_handler_ref (0)
,  _focus_flag (false)
,  _modifiers (0)
,  _inhibit_delta_flag (false)
,  _view_impl (*this)
{
   // kEventClassControl / kEventControlSetFocusPart
   // kEventParamControlPart typeControlPartCode
   
   OSStatus err = HIObjectCreate (kHIViewClassID, 0, (HIObjectRef *) &_hi_view_ref);
   assert (err == 0);
   
   static const EventTypeSpec view_event_type_arr [] = {
      {kEventClassMouse, kEventMouseDown},
      {kEventClassMouse, kEventMouseUp},
      {kEventClassMouse, kEventMouseMoved},
      {kEventClassMouse, kEventMouseDragged},
      //{kEventClassMouse, kEventMouseEntered},
      //{kEventClassMouse, kEventMouseExited},
      {kEventClassMouse, kEventMouseWheelMoved},
      {kEventClassControl, kEventControlBoundsChanged},
      {kEventClassControl, kEventControlDragEnter},
      {kEventClassControl, kEventControlDragWithin},
      {kEventClassControl, kEventControlDragLeave},
      {kEventClassControl, kEventControlDragReceive},
      {kEventClassControl, kEventControlTrackingAreaEntered},
      {kEventClassControl, kEventControlTrackingAreaExited},
      {kEventClassControl, kEventControlSetFocusPart},
      //{kEventClassControl, kEventControlGetFocusPart},
      {kEventClassTextInput, kEventTextInputUnicodeForKeyEvent},
      {kEventClassKeyboard, kEventRawKeyDown},
      {kEventClassKeyboard, kEventRawKeyRepeat},
      {kEventClassKeyboard, kEventRawKeyUp},
      {kEventClassKeyboard, kEventRawKeyModifiersChanged},
   };
   
   err = InstallControlEventHandler (
      _hi_view_ref,
      handle_event_proc,
      GetEventTypeCount (view_event_type_arr),
      view_event_type_arr,
      this,
      &_hi_view_event_handler_ref
   );
   assert (err == 0);
   
   WindowRef window_ref = window.get_system_window ();
   
   HIViewRef content_ref = EventFnc::use_instance ().get_content_view_ref (window_ref);
   
   err = HIViewAddSubview (content_ref, _hi_view_ref);
   assert (err == 0);
   
   // the tracking aera is automatically disposed with the view
   err = HIViewNewTrackingArea (
      _hi_view_ref,
      0, // all aera
      0, // id
      0
   );
   assert (err == 0);
   
   {
      ViewImpl * this_ptr = this;
      SetControlProperty (
         _hi_view_ref,
         _property_creator, _property_tag,
         sizeof (ViewImpl *), &this_ptr
      );
      assert (err == 0);
   }
   
   SetAutomaticControlDragTrackingEnabledForWindow (window_ref, true);

   SetControlDragTrackingEnabled (_hi_view_ref, true);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ViewImpl::~ViewImpl ()
{
   try
   {
      if (_hi_view_event_handler_ref != 0)
      {
         OSStatus err = RemoveEventHandler (_hi_view_event_handler_ref);
         assert (err == 0);
         _hi_view_event_handler_ref = 0;
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : vt_init
Thread : Video
==============================================================================
*/

void  ViewImpl::vt_init ()
{
   _view_impl.vt_init ();
}



/*
==============================================================================
Name : vt_restore
Thread : Video
==============================================================================
*/

void  ViewImpl::vt_restore ()
{
   _view_impl.vt_restore ();
}



/*
==============================================================================
Name : vt_update
Thread : Video
==============================================================================
*/

void  ViewImpl::vt_update (const opa::Size & size)
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

void  ViewImpl::vt_draw (bool wait_vbr_flag)
{
   _view_impl.vt_draw (wait_vbr_flag);
}



/*
==============================================================================
Name : get_system_view
==============================================================================
*/

HIViewRef   ViewImpl::get_system_view () const
{
   return _hi_view_ref;
}



/*
==============================================================================
Name : use_view
==============================================================================
*/

View &   ViewImpl::use_view ()
{
   return _view;
}



/*
==============================================================================
Name : use_view
==============================================================================
*/

ViewImpl *  ViewImpl::get (HIViewRef view_ref)
{
   ViewImpl * this_ptr = 0;
   
   OSStatus err = GetControlProperty (
      view_ref,
      _property_creator, _property_tag,
      sizeof (void *), 0,
      &this_ptr
   );
   assert ((err == 0) || (err == controlPropertyNotFoundErr));
   
   // this_ptr might be null
   
   return this_ptr;
}



/*
==============================================================================
Name : set_origin
==============================================================================
*/

void  ViewImpl::set_origin (const opa::Point & point)
{
   assert (_hi_view_ref != 0);
   
   CGRect rect;
   OSStatus err = HIViewGetFrame (_hi_view_ref, &rect);
   assert (err == 0);
   
   rect.origin = point.to_CGPoint ();
   
   err = HIViewSetFrame (_hi_view_ref, &rect);
   assert (err == 0);
}



/*
==============================================================================
Name : set_bounds
==============================================================================
*/

void  ViewImpl::set_bounds (const opa::Size & size)
{
   assert (_hi_view_ref != 0);
   
   CGRect rect;
   OSStatus err = HIViewGetFrame (_hi_view_ref, &rect);
   assert (err == 0);
   
   rect.size = size.to_CGSize ();
   
   err = HIViewSetFrame (_hi_view_ref, &rect);
   assert (err == 0);
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

opa::Size   ViewImpl::get_bounds ()
{
   return _bounds;
}



/*
==============================================================================
Name : post_view_layout_changed
Description :
   Post a view layout changed event into the window or application message
   queue (e.g. not directly).
   This is used to ensure that :
   - a view layout changed msg is sent at the construction level before
     the initial application run
   - a view layout changed msg is sent after the current msg propagation
==============================================================================
*/

void  ViewImpl::post_view_layout_changed ()
{
   EventRef event_ref = 0;
   OSStatus err = CreateEvent (
      0,
      kEventClassControl, kEventControlBoundsChanged,
      GetCurrentEventTime (), kEventAttributeNone,
      &event_ref
   );
   assert (err == 0);
   
   err = SetEventParameter (event_ref, kEventParamDirectObject, typeControlRef, sizeof (HIViewRef), &_hi_view_ref);
   assert (err == 0);
   
   PostEventToQueue (GetMainEventQueue (), event_ref, kEventPriorityStandard);
}



/*
==============================================================================
Name : show
==============================================================================
*/

void  ViewImpl::show ()
{
   HIViewSetVisible (_hi_view_ref, true);
}



/*
==============================================================================
Name : hide
==============================================================================
*/

void  ViewImpl::hide ()
{
   HIViewSetVisible (_hi_view_ref, false);
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  ViewImpl::invalidate (const opa::Rect & rect)
{
   _view_impl.invalidate (rect);
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  ViewImpl::invalidate_all ()
{
#if defined (OHM_OPAL_NO_INVALIDATION)
   return;
#endif

   opa::Rect rect;
   rect._origin = opa::Point::_zero;
   rect._size = _view.get_bounds ();
   
   invalidate (rect);
}



/*
==============================================================================
Name : acquire_keyboard_focus
==============================================================================
*/

void  ViewImpl::acquire_keyboard_focus ()
{
   assert (_hi_view_ref != 0);
   
   WindowRef window_ref = HIViewGetWindow (_hi_view_ref);
   assert (window_ref != 0);
   
   OSErr err = SetKeyboardFocus (window_ref, _hi_view_ref, kControlFocusNextPart);
   assert (err == 0);
}


/*
==============================================================================
Name : release_keyboard_focus
==============================================================================
*/

void  ViewImpl::release_keyboard_focus ()
{
   assert (_hi_view_ref != 0);
   
   WindowRef window_ref = HIViewGetWindow (_hi_view_ref);
   assert (window_ref != 0);
   
   ClearKeyboardFocus (window_ref);
   
   /*OSErr err = SetKeyboardFocus (window_ref, _hi_view_ref, kControlFocusNoPart);
   assert (err == 0);*/
}



/*
==============================================================================
Name : begin_mouse_capture
==============================================================================
*/

void  ViewImpl::begin_mouse_capture (bool relative_hidden_flag, bool block_flag)
{
   ApplicationImpl::use_instance ().begin_mouse_capture (_hi_view_ref, relative_hidden_flag, block_flag);
}



/*
==============================================================================
Name : end_mouse_capture
==============================================================================
*/

void  ViewImpl::end_mouse_capture ()
{
   ApplicationImpl::use_instance ().end_mouse_capture (_hi_view_ref);
}



/*
==============================================================================
Name : end_mouse_capture
==============================================================================
*/

void  ViewImpl::end_mouse_capture (const opa::Point & view_rel_pt)
{
   ApplicationImpl::use_instance ().end_mouse_capture (_hi_view_ref, view_rel_pt);
   
   _inhibit_delta_flag = true;
}



/*
==============================================================================
Name : begin_mouse_gesture
==============================================================================
*/

void  ViewImpl::begin_mouse_gesture (bool block_flag)
{
   ApplicationImpl::use_instance ().begin_mouse_gesture (_hi_view_ref, block_flag);
}



/*
==============================================================================
Name : end_mouse_gesture
==============================================================================
*/

void  ViewImpl::end_mouse_gesture ()
{
   ApplicationImpl::use_instance ().end_mouse_gesture (_hi_view_ref);
}



/*
==============================================================================
Name : debug_toggle_show_invalid_rect
==============================================================================
*/

void  ViewImpl::debug_toggle_show_invalid_rect ()
{
   _view_impl.debug_toggle_show_invalid_rect ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : handle_event_proc
==============================================================================
*/

OSStatus ViewImpl::handle_event_proc (EventHandlerCallRef /*handler_call_ref*/, EventRef event_ref, void * user_data)
{
   OSStatus ret_val = noErr;
   
   try
   {
      ViewImpl * this_ptr = (ViewImpl *) user_data;
      
      ret_val = this_ptr->handle_event (event_ref);
   }
   catch (...)
   {
      assert (false);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_event
==============================================================================
*/

OSStatus ViewImpl::handle_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventClass (event_ref))
   {
   case kEventClassMouse:
      ret_val = handle_mouse_event (event_ref);
      break;

   case kEventClassControl:
      ret_val = handle_control_event (event_ref);
      break;
   
   case kEventClassKeyboard:
      ret_val = handle_raw_keyboard_event (event_ref);
      break;

   case kEventClassTextInput:
      ret_val = handle_text_input_event (event_ref);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_mouse_event
==============================================================================
*/

OSStatus ViewImpl::handle_mouse_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   bool ok_flag = !task::RunLoopGui::use_instance ().is_processing_event ();
   
   switch (GetEventKind (event_ref))
   {
   case kEventMouseDown:
      acquire_keyboard_focus ();
      
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         const int mouse_button 
            = EventFnc::use_instance ().get_mouse_button (event_ref);
         // mouse_button value is 1 or 2
         
         if (mouse_button >= 1 && mouse_button <= 2)
         {
            _sys_event._mouse_location = EventFnc::use_instance ().get_mouse_view_location (_hi_view_ref, event_ref);
            _sys_event._click_cnt_arr [mouse_button - 1] 
               = EventFnc::use_instance ().get_click_count (event_ref);
            
            _view.handle_system_event (_sys_event);
         }
#else
         _view.mouse_moved (
            EventFnc::use_instance ().get_mouse_view_location (_hi_view_ref, event_ref),
            opa::Point::_zero
         );
         
         _view.mouse_button_pressed (
            EventFnc::use_instance ().get_mouse_button (event_ref),
            EventFnc::use_instance ().get_click_count (event_ref)
         );
#endif         
         run_loop.notify_post_gui_event ();
      }
      break;

   case kEventMouseUp:
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         const int mouse_button 
            = EventFnc::use_instance ().get_mouse_button (event_ref);
         
         if (mouse_button >= 1 && mouse_button <= 2)
         {
            _sys_event._click_cnt_arr [mouse_button - 1] = 0;
            
            _view.handle_system_event (_sys_event);
         }
#else
         _view.mouse_button_released (
            EventFnc::use_instance ().get_mouse_button (event_ref)
         );
#endif         
         run_loop.notify_post_gui_event ();
      }
      break;

   case kEventMouseMoved:
   case kEventMouseDragged:
      {
         opa::Point view_location = EventFnc::use_instance ().get_mouse_view_location (_hi_view_ref, event_ref);
         
         CGRect view_rect;
         HIViewGetBounds (_hi_view_ref, &view_rect);
         
         opa::Rect rect;
         rect.from_CGRect (view_rect);

#if defined ohm_opal_VIEW_USE_EVENT_MGR
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
            
            _sys_event._mouse_location = view_location;
            
            if (! rect.contains (view_location))
            {
               _sys_event._mouse_exited_view_flag = true; 
            }
            
            _view.handle_system_event (_sys_event);

            run_loop.notify_post_gui_event ();
         }
#else
         opa::Point mouse_delta (0, 0);
         
         if (!_inhibit_delta_flag)
         {
            mouse_delta = EventFnc::use_instance ().get_mouse_delta (event_ref);
         }
         
         _inhibit_delta_flag = false;
         
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
            
            _view.mouse_moved (
               view_location,
               mouse_delta
            );

            run_loop.notify_post_gui_event ();
         }
         
         if (!rect.contains (view_location))
         {
            if (ok_flag)
            {
               run_loop.notify_pre_gui_event ();
               
               _view.mouse_exited_view (
                  view_location,
                  mouse_delta
               );

               run_loop.notify_post_gui_event ();
            }
         }
#endif
      }
      break;

   case kEventMouseWheelMoved:
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._wheel_delta
            = EventFnc::use_instance ().get_wheel_delta (event_ref);
            
         _view.handle_system_event (_sys_event);
#else
         _view.mouse_wheel_moved (
            EventFnc::use_instance ().get_wheel_delta (event_ref)
         );
#endif

         run_loop.notify_post_gui_event ();
      }
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_control_event
==============================================================================
*/

OSStatus ViewImpl::handle_control_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   bool ok_flag = !task::RunLoopGui::use_instance ().is_processing_event ();
   
   switch (GetEventKind (event_ref))
   {
   case kEventControlBoundsChanged:
      {
         assert (_hi_view_ref != 0);
   
         CGRect rect;
         OSStatus err = HIViewGetFrame (_hi_view_ref, &rect);
         assert (err == 0);
         
         opa::Size ret_val;
         _bounds.from_CGSize (rect.size);

#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._layout_changed_flag = true;
         _view.handle_system_event (_sys_event);
#else
         _view.view_layout_changed ();
#endif
      }
      break;

   case kEventControlTrackingAreaEntered:
      // nothing
      break;

   case kEventControlTrackingAreaExited:
      // the mouse param is in view coordinates
      if (ActiveNonFloatingWindow () == _window.get_system_window ())
      {
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
            
#if defined ohm_opal_VIEW_USE_EVENT_MGR
            _sys_event._mouse_location 
               = EventFnc::use_instance ().get_mouse_location (event_ref);
            _sys_event._mouse_exited_view_flag = true; 

            _view.handle_system_event (_sys_event);
#else
            _view.mouse_exited_view (
               EventFnc::use_instance ().get_mouse_location (event_ref),
               opa::Point (0, 0)
            );
#endif
            run_loop.notify_post_gui_event ();
         }
      }
#if 0
      InitCursor ();
#elif 1
      [[NSCursor arrowCursor] set];
#endif
      break;
   
   case kEventControlDragEnter:
      {
         DragRef drag_ref = EventFnc::use_instance ().get_drag_ref (event_ref);
         
         process_drag (_drag_data, drag_ref);
         
         opa::Point mouse_location = get_drag_location (drag_ref);
         
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
                        
            _view.mouse_data_drag_set (_drag_data);

#if defined ohm_opal_VIEW_USE_EVENT_MGR
            _sys_event._has_drag_data_flag = true;
            _view.handle_system_event (_sys_event);
#else
            _view.mouse_moved (
               mouse_location,
               opa::Point (0, 0)
            );
#endif
            run_loop.notify_post_gui_event ();
         }
         
         // needed to receive kEventControlDragWithin & kEventControlDragLeave
         
         ::Boolean accept_drop_flag = true;
         
         OSStatus err = SetEventParameter (
            event_ref,
            kEventParamControlWouldAcceptDrop,
            typeBoolean,
            sizeof (::Boolean),
            &accept_drop_flag
         );
         assert (err == 0);
         
         // we need to send this to continue to receive items
         ret_val = noErr;
      }
      break;
      
   case kEventControlDragWithin:
      {
         DragRef drag_ref = EventFnc::use_instance ().get_drag_ref (event_ref);
         
         opa::Point mouse_location = get_drag_location (drag_ref);
         
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
                        
            _view.mouse_data_drag_set (_drag_data);

#if defined ohm_opal_VIEW_USE_EVENT_MGR
            _sys_event._mouse_location = mouse_location;
            _sys_event._has_drag_data_flag = true;
            _view.handle_system_event (_sys_event);
#else
            _view.mouse_moved (
               mouse_location,
               opa::Point (0, 0)
            );
#endif
            run_loop.notify_post_gui_event ();
         }
         
         ret_val = noErr;
      }
      break;
      
   case kEventControlDragLeave:
      {
         DragRef drag_ref = EventFnc::use_instance ().get_drag_ref (event_ref);
         
         opa::Point mouse_location = get_drag_location (drag_ref);
         
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
            
            _view.mouse_data_drag_set (_drag_data);
            
#if defined ohm_opal_VIEW_USE_EVENT_MGR
            _sys_event._has_drag_data_flag = true;
            _sys_event._mouse_exited_view_flag = true; 
            _sys_event._mouse_location = mouse_location;
            _view.handle_system_event (_sys_event);
#else
            _view.mouse_exited_view (
               mouse_location,
               opa::Point (0, 0)
            );
#endif            
            run_loop.notify_post_gui_event ();
            
            _drag_data.clear ();
         }
         
         ret_val = noErr;
      }
      break;
      
   case kEventControlDragReceive:
      {
         DragRef drag_ref = EventFnc::use_instance ().get_drag_ref (event_ref);
         
         opa::Point mouse_location = get_drag_location (drag_ref);
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();

            _view.mouse_data_drag_set (_drag_data);
            
            _sys_event._key = Key (event_ref);
            _sys_event._mouse_location = mouse_location;
            _sys_event._drag_data_dropped_flag = true;
            
            if (ActiveNonFloatingWindow () != _window.get_system_window ())
            {
               _sys_event._mouse_exited_view_flag = true;
            }
            
            _view.handle_system_event (_sys_event);
            
            run_loop.notify_post_gui_event ();
         }
#else
         if (ok_flag)
         {
            run_loop.notify_pre_gui_event ();
            
            _view.mouse_data_drag_set (_drag_data);
            _view.mouse_data_dropped (
               mouse_location,
               opa::Point (0, 0)
            );

            run_loop.notify_post_gui_event ();
         }
         
         if (ActiveNonFloatingWindow () != _window.get_system_window ())
         {
            if (ok_flag)
            {
               run_loop.notify_pre_gui_event ();
               
               _view.mouse_exited_view (
                  mouse_location,
                  opa::Point (0, 0)
               );

               run_loop.notify_post_gui_event ();
            }
         }
#endif         
         ret_val = noErr;
      }
      // basically this means that some drag come from the system
      // put the data at application level accordingly
      // this is the only thing we should do
      break;
   
   case kEventControlSetFocusPart:
      {
         ControlPartCode part = EventFnc::use_instance ().get_control_part_code (event_ref);
         
         if (part == kControlFocusNoPart)
         {
            if (ok_flag)
            {
               run_loop.notify_pre_gui_event ();
               
#if defined ohm_opal_VIEW_USE_EVENT_MGR
               _sys_event._keyboard_focus_lost_flag = true;
               _view.handle_system_event (_sys_event);
#else
               _view.keyboard_focus_lost ();
#endif               
               run_loop.notify_post_gui_event ();
            }
         }
         
         ret_val = noErr;
      }
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_text_input_event
==============================================================================
*/

OSStatus ViewImpl::handle_raw_keyboard_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   bool catched_flag = false;
   
   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   bool ok_flag = !task::RunLoopGui::use_instance ().is_processing_event ();
   
   switch (GetEventKind (event_ref))
   {
   case kEventRawKeyDown:
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._key = Key (event_ref);
         _sys_event._key_change = SystemEvent::KeyChange_PRESSED;
         catched_flag = _view.handle_system_event (_sys_event);
#else
         catched_flag = _view.key_pressed (
            Key (event_ref)
         );
#endif
         run_loop.notify_post_gui_event ();
      }
      break;

   case kEventRawKeyRepeat:
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._key = Key (event_ref);
         _sys_event._key_change = SystemEvent::KeyChange_REPEATED;
         catched_flag = _view.handle_system_event (_sys_event);
#else
         catched_flag = _view.key_repeated (
            Key (event_ref)
         );
#endif
         run_loop.notify_post_gui_event ();
      }
      break;

   case kEventRawKeyUp:
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._key = Key (event_ref);
         _sys_event._key_change = SystemEvent::KeyChange_RELEASED;
         catched_flag = _view.handle_system_event (_sys_event);
#else
         catched_flag = _view.key_released (
            Key (event_ref)
         );
#endif
         run_loop.notify_post_gui_event ();
      }
      break;

   case kEventRawKeyModifiersChanged:
      {
         archi::UInt32 modifiers
            = EventFnc::use_instance ().get_key_modifiers (event_ref);
         
         if (ok_flag)
         {
            if (modifiers > _modifiers)
            {
               run_loop.notify_pre_gui_event ();
               
#if defined ohm_opal_VIEW_USE_EVENT_MGR
               _sys_event._key = Key (event_ref);
               _sys_event._key_change = SystemEvent::KeyChange_PRESSED;
               catched_flag = _view.handle_system_event (_sys_event);
#else
               catched_flag = _view.key_pressed (
                  Key (event_ref)
               );
#endif
               run_loop.notify_post_gui_event ();
            }
            else if (modifiers < _modifiers)
            {
               run_loop.notify_pre_gui_event ();
               
#if defined ohm_opal_VIEW_USE_EVENT_MGR
               _sys_event._key = Key (event_ref);
               _sys_event._key_change = SystemEvent::KeyChange_RELEASED;
               catched_flag = _view.handle_system_event (_sys_event);
#else
               catched_flag = _view.key_released (
                  Key (event_ref)
               );
#endif
               run_loop.notify_post_gui_event ();
            }
            else
            {
               // nothing
            }
         }
         
         _modifiers = modifiers;
      }
      break;
   }
   
   if (catched_flag)
   {
      ret_val = noErr;
   }
   else
   {
      ret_val = eventNotHandledErr;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_text_input_event
==============================================================================
*/

OSStatus ViewImpl::handle_text_input_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;

   bool catched_flag = false;

   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   bool ok_flag = !task::RunLoopGui::use_instance ().is_processing_event ();
   
   switch (GetEventKind (event_ref))
   {
   case kEventTextInputUnicodeForKeyEvent:
      if (ok_flag)
      {
         run_loop.notify_pre_gui_event ();
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._char_utf8 
            = EventFnc::use_instance ().get_characters (event_ref);
         catched_flag = _view.handle_system_event (_sys_event);
#else
         catched_flag = _view.characters_typed (
            EventFnc::use_instance ().get_characters (event_ref)
         );
#endif
         run_loop.notify_post_gui_event ();
      }
      break;
   }

   if (catched_flag)
   {
      ret_val = noErr;
   }
   else
   {
      ret_val = eventNotHandledErr;
   }
   
   return ret_val;
}



/*
==============================================================================
Name: get_drag_location
==============================================================================
*/

opa::Point  ViewImpl::get_drag_location (DragRef drag_ref)
{
   ::Point qd_location;
         
   OSErr err = GetDragMouse (
      drag_ref,
      &qd_location,
      0
   );
   assert (err == 0);
   
   opa::Point mouse_location;
   mouse_location._x = qd_location.h;
   mouse_location._y = qd_location.v;
   
   mouse_location = EventFnc::use_instance ()
      .get_mouse_view_location (_hi_view_ref, mouse_location);
      
   return mouse_location;
}



/*
==============================================================================
Name: process_drag
==============================================================================
*/

void  ViewImpl::process_drag (Data & data, DragRef drag_ref)
{
   assert (drag_ref != 0);
   
   ::UInt16 num_items = 0;
   
   OSErr err = CountDragItems (drag_ref, &num_items);
   assert (err == 0);
   
   for (::UInt16 i = 0 ; i < num_items ; ++i)
   {
      DragItemRef drag_item_ref = 0;
      
      err = GetDragItemReferenceNumber (
         drag_ref,
         i + 1,
         &drag_item_ref
      );
      assert (err == 0);
      assert (drag_item_ref != 0);
      
      Data::MimeTypeDataMap & mime_type_data_map = *data._mime_type_data_map_arr.insert (
         data._mime_type_data_map_arr.end (), Data::MimeTypeDataMap ()
      );
      
      process_drag_item (mime_type_data_map, drag_ref, drag_item_ref);
   }
}



/*
==============================================================================
Name: process_drag_item
==============================================================================
*/

void  ViewImpl::process_drag_item (Data::MimeTypeDataMap & mime_type_data_map, DragRef drag_ref, DragItemRef drag_item_ref)
{
   assert (drag_ref != 0);
   assert (drag_item_ref != 0);
   
   ::UInt16 num_flavors = 0;
   
   OSErr err = CountDragItemFlavors (
      drag_ref,
      drag_item_ref,
      &num_flavors
   );
   assert (err == 0);
   
   for (::UInt16 i = 0 ; i < num_flavors ; ++i)
   {
      FlavorType flavor_type;
      
      err = GetFlavorType (
         drag_ref,
         drag_item_ref,
         i + 1,
         &flavor_type
      );
      assert (err == 0);
      
      std::string mime_type = to_mime_type (flavor_type);
      
      if (mime_type != "")
      {
         ::Size data_size;
         err = GetFlavorDataSize (
            drag_ref,
            drag_item_ref,
            flavor_type,
            &data_size
         );
         assert (err == 0);
         
         if (data_size > 0)
         {
            std::vector <archi::UByte> & raw_data
               = mime_type_data_map [mime_type];

            raw_data.resize (data_size);
            
            err = GetFlavorData (
               drag_ref,
               drag_item_ref,
               flavor_type,
               &raw_data [0],
               &data_size,
               0  // data offset
            );
            assert (err == 0);
         }
      }
   }
}



/*
==============================================================================
Name: to_mime_type
==============================================================================
*/

std::string ViewImpl::to_mime_type (FlavorType flavor_type)
{
   std::string ret_val;
   
   switch (flavor_type)
   {
   case typeFileURL:
      ret_val = Data::_uti_file_url_0;
      break;
   
   case keyAEContainer:
      break;

   case kDragFlavorTypeHFS:
      break;
   }
   
   return ret_val;
}



PropertyCreator   ViewImpl::_property_creator = 'OmFo';
PropertyTag       ViewImpl::_property_tag = 'this';



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
