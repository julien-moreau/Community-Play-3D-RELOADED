/*****************************************************************************

        WindowImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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

#include "ohm/opal/carbon/WindowImpl.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/View.h"
#include "ohm/opal/carbon/ScreensImpl.h"
#include "ohm/opal/carbon/EventFnc.h"
#include "ohm/opal/View.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/carbon/RunLoopGuiImpl.h"

#include <cassert>



namespace ohm
{
namespace opal
{


// for Windows transparent windows :
// http://social.msdn.microsoft.com/forums/en-US/wpf/thread/e6c0a499-9104-4971-ad5f-949dbe4de910/
// UpdateLayeredWindow & DirectX
// http://msdn.microsoft.com/en-us/library/ms633556(v=vs.85).aspx

// to make a snapshot of a window :
// CGWindowListCreateImage

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Creates a window with dummy rects, and keep it hidded.
==============================================================================
*/

WindowImpl::WindowImpl (Window & window)
:  _window (window)
,  _window_ref (0)
,  _window_class (kDocumentWindowClass)
,  _group_window_class (kDocumentWindowClass)
,  _shadow_flag (true)
,  _window_attributes (kWindowStandardHandlerAttribute | kWindowCompositingAttribute)
,  _window_event_handler_ref (0)
,  _target_view_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

WindowImpl::~WindowImpl ()
{
   if (_window_event_handler_ref != 0)
   {
      OSStatus err = RemoveEventHandler (_window_event_handler_ref);
      assert (err == 0);
      _window_event_handler_ref = 0;
   }
   
   if (_window_ref != 0)
   {
      DisposeWindow (_window_ref);
      _window_ref = 0;
   }
}



/*
==============================================================================
Name : set_class_document
==============================================================================
*/

void  WindowImpl::set_class_document ()
{
   _window_class = kDocumentWindowClass;
   _group_window_class = kDocumentWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_class_floating
==============================================================================
*/

void  WindowImpl::set_class_floating ()
{
   _window_class = kFloatingWindowClass;
   _group_window_class = kFloatingWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_class_floating_overlay_group
==============================================================================
*/

void  WindowImpl::set_class_floating_overlay_group ()
{
   _window_class = kUtilityWindowClass;
   _group_window_class = kUtilityWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_class_floating_no_decoration
==============================================================================
*/

void  WindowImpl::set_class_floating_no_decoration ()
{
   _window_class = kSimpleWindowClass;
   _group_window_class = kFloatingWindowClass;

   _window_attributes |= kWindowNoShadowAttribute;
   
   update ();
}



/*
==============================================================================
Name : set_class_no_decoration
==============================================================================
*/

void  WindowImpl::set_class_no_decoration ()
{
   _window_class = kSimpleWindowClass;
   _group_window_class = kSimpleWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_class_dialog
==============================================================================
*/

void  WindowImpl::set_class_dialog ()
{
   _window_class = kModalWindowClass;
   _group_window_class = kModalWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_class_overlay
==============================================================================
*/

void  WindowImpl::set_class_overlay ()
{
   _window_class = kOverlayWindowClass;
   _group_window_class = kOverlayWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_overlay_group
==============================================================================
*/

void  WindowImpl::set_overlay_group ()
{
   _group_window_class = kOverlayWindowClass;
   
   update ();
}



/*
==============================================================================
Name : set_no_shadow
==============================================================================
*/

void  WindowImpl::set_no_shadow ()
{
   _shadow_flag = false;
   
   update ();
}



/*
==============================================================================
Name : set_properties
==============================================================================
*/

void  WindowImpl::set_properties (bool close_box_flag, bool zoom_box_flag, bool collapse_box_flag, bool resizable_flag)
{
   if (close_box_flag)
   {
      _window_attributes |= kWindowCloseBoxAttribute;
   }
   
   if (zoom_box_flag)
   {
      _window_attributes |= kWindowFullZoomAttribute;
   }
   
   if (collapse_box_flag)
   {
      _window_attributes |= kWindowCollapseBoxAttribute;
   }
   
   if (resizable_flag)
   {
      _window_attributes |= kWindowResizableAttribute;
      _window_attributes |= kWindowLiveResizeAttribute;
   }

   update ();
}



/*
==============================================================================
Name : is_class_document
==============================================================================
*/

bool  WindowImpl::is_class_document () const
{
   return (_window_class == kDocumentWindowClass);
}



/*
==============================================================================
Name : create
Description :
   Create the window and attach event handlers.
   Event handlers listen to event that are dispatched to the views.
   Those events are not normally dispatched to views, so that's why we
   dispatch them manually.
   The window is created 1 pixel wide. It must be resized before it is shown.
==============================================================================
*/

void  WindowImpl::create ()
{
   ::Rect window_rect;
   window_rect.left = 0;
   window_rect.top = 0;
   window_rect.right = 1;
   window_rect.bottom = 1;
   
   OSStatus err = CreateNewWindow (
      _window_class,
      _window_attributes,
      &window_rect,
      &_window_ref
   );
   assert (err == 0);
   assert (_window_ref != 0);
   
   if (_window_class == kFloatingWindowClass)
   {
      // remove hide on suspend
      
      err = ChangeWindowAttributes (
         _window_ref, kWindowNoAttributes, kWindowHideOnSuspendAttribute
      );
      assert (err == 0);
   }
   
   if (!_shadow_flag)
   {
      // set no shadow
      
      err = ChangeWindowAttributes (
         _window_ref, kWindowNoShadowAttribute, kWindowNoAttributes
      );
      assert (err == 0);
   }
   
   if (_window_class != _group_window_class)
   {
      WindowGroupRef window_group_ref = GetWindowGroupOfClass (_group_window_class);
      assert (window_group_ref != 0);
      
      err = SetWindowGroup (_window_ref, window_group_ref);
      assert (err == 0);
   }

   static const EventTypeSpec window_event_type_arr [] = {
      {kEventClassMouse, kEventMouseDown},
      {kEventClassMouse, kEventMouseUp},
      {kEventClassMouse, kEventMouseMoved},
      {kEventClassMouse, kEventMouseDragged},
      {kEventClassMouse, kEventMouseWheelMoved},

      {kEventClassWindow, kEventWindowBoundsChanged},
      {kEventClassWindow, kEventWindowClose},
   };
   
   err = InstallWindowEventHandler (
      _window_ref,
      handle_event_proc,
      GetEventTypeCount (window_event_type_arr),
      window_event_type_arr,
      this,
      &_window_event_handler_ref
   );
   assert (err == 0);
   
   WindowImpl * this_ptr = this;

   SetWindowProperty (
      _window_ref,
      _property_creator, _property_tag,
      sizeof (void *), &this_ptr
   );
}



/*
==============================================================================
Name : show
==============================================================================
*/

void  WindowImpl::show ()
{
   assert (_window_ref != 0);
   
   ShowWindow (_window_ref);
}



/*
==============================================================================
Name : hide
==============================================================================
*/

void  WindowImpl::hide ()
{
   HideWindow (_window_ref);
}



/*
==============================================================================
Name : collapse
==============================================================================
*/

void  WindowImpl::collapse ()
{
   assert (_window_ref != 0);
   
   #warning won't work on simple window class
   CollapseWindow (_window_ref, true);
}



/*
==============================================================================
Name : set_origin
==============================================================================
*/

void  WindowImpl::set_origin (const opa::Point & origin)
{
   assert (_window_ref != 0);
   
   ::Rect window_rect;
   
   OSStatus err = GetWindowBounds (_window_ref, kWindowStructureRgn, &window_rect);
   assert (err == 0);
   
   ::Rect new_window_rect;
   new_window_rect.left = int (origin._x);
   new_window_rect.top = int (origin._y);
   new_window_rect.right = new_window_rect.left + window_rect.right - window_rect.left;
   new_window_rect.bottom = new_window_rect.top + window_rect.bottom - window_rect.top;
   
   err = SetWindowBounds (_window_ref, kWindowStructureRgn, &new_window_rect);
   assert (err == 0);
}



/*
==============================================================================
Name : get_origin
==============================================================================
*/

opa::Point  WindowImpl::get_origin ()
{
   assert (_window_ref != 0);
   
   ::Rect window_rect;
   
   OSStatus err = GetWindowBounds (_window_ref, kWindowStructureRgn, &window_rect);
   assert (err == 0);
   
   opa::Point ret_val (window_rect.left, window_rect.top);
   
   return ret_val;
}



/*
==============================================================================
Name : get_content_origin
==============================================================================
*/

opa::Point  WindowImpl::get_content_origin ()
{
   assert (_window_ref != 0);
   
   ::Rect window_rect;
   
   OSStatus err = GetWindowBounds (_window_ref, kWindowContentRgn, &window_rect);
   assert (err == 0);
   
   opa::Point ret_val (window_rect.left, window_rect.top);
   
   return ret_val;
}



/*
==============================================================================
Name : reposition_center
==============================================================================
*/

void  WindowImpl::reposition_center (Window * parent_window_ptr, bool screen_relative_flag)
{
   assert (_window_ref != 0);
   
   if (parent_window_ptr != 0)
   {
      WindowRef parent_window_ref = parent_window_ptr->get_system_window ();
      
      if (screen_relative_flag)
      {
         RepositionWindow (_window_ref, parent_window_ref, kWindowCenterOnParentWindowScreen);
      }
      else
      {
         RepositionWindow (_window_ref, parent_window_ref, kWindowCenterOnParentWindow);
      }
   }
   else
   {
      RepositionWindow (_window_ref, 0, kWindowCenterOnMainScreen);
   }
}



/*
==============================================================================
Name : reposition_alert
==============================================================================
*/

void  WindowImpl::reposition_alert (Window * parent_window_ptr, bool screen_relative_flag)
{
   assert (_window_ref != 0);
   
   if (parent_window_ptr != 0)
   {
      WindowRef parent_window_ref = parent_window_ptr->get_system_window ();
      
      if (screen_relative_flag)
      {
         RepositionWindow (_window_ref, parent_window_ref, kWindowAlertPositionOnParentWindowScreen);
      }
      else
      {
         RepositionWindow (_window_ref, parent_window_ref, kWindowAlertPositionOnParentWindow);
      }
   }
   else
   {
      RepositionWindow (_window_ref, 0, kWindowAlertPositionOnMainScreen);
   }
}



/*
==============================================================================
Name : resize_default_main_screen
==============================================================================
*/

void  WindowImpl::resize_default_main_screen ()
{
   opa::Rect rect = ScreensImpl::get_main_screen_without_decorations ();
   
   const float space_hori = 3;
   
   ::Rect new_window_rect;
   new_window_rect.left = int (rect._origin._x + space_hori);
   new_window_rect.top = int (rect._origin._y);
   new_window_rect.right = int (rect._origin._x + rect._size._width - space_hori);
   new_window_rect.bottom = int (rect._origin._y + rect._size._height);
   
   OSStatus err = SetWindowBounds (_window_ref, kWindowStructureRgn, &new_window_rect);
   assert (err == 0);
}



/*
==============================================================================
Name : set_bounds
==============================================================================
*/

void  WindowImpl::set_bounds (const opa::Size & bounds)
{
   assert (_window_ref != 0);
   
   ::Rect window_rect;
   
   OSStatus err = GetWindowBounds (_window_ref, kWindowContentRgn, &window_rect);
   assert (err == 0);
   
   ::Rect new_window_rect;
   new_window_rect.left = window_rect.left;
   new_window_rect.top = window_rect.top;
   new_window_rect.right = window_rect.left + int (bounds._width);
   new_window_rect.bottom = window_rect.top + int (bounds._height);
   
   err = SetWindowBounds (_window_ref, kWindowContentRgn, &new_window_rect);
   assert (err == 0);
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

opa::Size   WindowImpl::get_bounds ()
{
   assert (_window_ref != 0);
   
   ::Rect window_rect;
   
   OSStatus err = GetWindowBounds (_window_ref, kWindowContentRgn, &window_rect);
   assert (err == 0);
   
   opa::Size ret_val (
      window_rect.right - window_rect.left,
      window_rect.bottom - window_rect.top
   );
   
   return ret_val;
}



/*
==============================================================================
Name : set_limit_bounds
==============================================================================
*/

void  WindowImpl::set_limit_bounds (const opa::Size & min_bounds, const opa::Size & max_bounds)
{
   assert (_window_ref != 0);
   
   CGSize min_size = min_bounds.to_CGSize ();
   CGSize max_size = max_bounds.to_CGSize ();
   
   OSStatus err = SetWindowResizeLimits (
      _window_ref, 
      &min_size, 
      max_bounds == opa::Size::_infinity ? 0 : &max_size
   );
   
   assert (err == 0);
}



/*
==============================================================================
Name : is_active
==============================================================================
*/

bool  WindowImpl::is_active ()
{
   assert (_window_ref != 0);
   bool ret_val = IsWindowActive (_window_ref);
   
   return ret_val;
}
   


/*
==============================================================================
Name : set_title
==============================================================================
*/

void  WindowImpl::set_title (const std::string & title)
{
   assert (_window_ref != 0);
   
   CFStringRef title_ref = CFStringCreateWithCString (0, title.c_str (), kCFStringEncodingUTF8);
   assert (title_ref != 0);
   
   OSStatus err = SetWindowTitleWithCFString (_window_ref, title_ref);
   assert (err == 0);
   
   CFRelease (title_ref);
   title_ref = 0;
}



/*
==============================================================================
Name : set_alpha
==============================================================================
*/

void  WindowImpl::set_alpha (float alpha)
{
   OSStatus err = SetWindowAlpha (_window_ref, alpha);
   assert (err == 0);
}



/*
==============================================================================
Name : set_modified
==============================================================================
*/

void  WindowImpl::set_modified (bool modified_flag)
{
   OSStatus err = SetWindowModified (_window_ref, modified_flag);
   assert (err == 0);
}



/*
==============================================================================
Name : set_proxy_path
==============================================================================
*/

void  WindowImpl::set_proxy_path (const std::string & proxy_path)
{
   FSRef fs_ref;
   OSStatus err = FSPathMakeRef ((::UInt8 *) proxy_path.c_str (), &fs_ref, 0);
   assert (err == 0);
   
   err = HIWindowSetProxyFSRef (_window_ref, &fs_ref);
   assert (err == 0);
}



/*
==============================================================================
Name : window_bounds_changed
Description :
   Notify all child views of window bounds change.
==============================================================================
*/

void  WindowImpl::window_bounds_changed ()
{
   assert (_window_ref != 0);
   
   // do not notify for pre/post gui event
   // window bounds changed trigger an event without
   // rescheduling it

   HIViewRef content_ref = EventFnc::use_instance ().get_content_view_ref (_window_ref);
   
   HIViewRef view_ref = HIViewGetFirstSubview (content_ref);
   
   while (view_ref != 0)
   {
      ViewImpl * view_impl_ptr = ViewImpl::get (view_ref);
      
      if (view_impl_ptr != 0)
      {
         View & view = view_impl_ptr->use_view ();

         view.window_layout_changed ();
      }
      
      view_ref = HIViewGetNextView (view_ref);
   }
}



/*
==============================================================================
Name : user_input_lost
Description :
   Notify all child views that the user input have been lost.
==============================================================================
*/

void  WindowImpl::user_input_lost ()
{
   assert (_window_ref != 0);
   
   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   bool ok_flag = !task::RunLoopGui::use_instance ().is_processing_event ();
   
   if (ok_flag)
   {
      run_loop.notify_pre_gui_event ();
      
      HIViewRef content_ref
         = EventFnc::use_instance ().get_content_view_ref (_window_ref);
      
      HIViewRef view_ref = HIViewGetFirstSubview (content_ref);
      
      while (view_ref != 0)
      {
         ViewImpl * view_impl_ptr = ViewImpl::get (view_ref);
         
         if (view_impl_ptr != 0)
         {
            View & view = view_impl_ptr->use_view ();

            view.user_input_lost ();
         }
         
         view_ref = HIViewGetNextView (view_ref);
      }
      
      run_loop.notify_post_gui_event ();
   }
}



/*
==============================================================================
Name : get_system_window
==============================================================================
*/

WindowRef   WindowImpl::get_system_window () const
{
   assert (_window_ref != 0);
   
   return _window_ref;
}



/*
==============================================================================
Name : get_window
==============================================================================
*/

Window & WindowImpl::get_window ()
{
   return _window;
}



/*
==============================================================================
Name : get
Description :
   Returns the 'WindowImpl' associated to 'window_ref' or 0 if there is none.
==============================================================================
*/

WindowImpl *   WindowImpl::get (WindowRef window_ref)
{
   WindowImpl * this_ptr = 0;
   
   OSStatus err = GetWindowProperty (
      window_ref,
      _property_creator, _property_tag,
      sizeof (void *), 0,
      &this_ptr
   );
   assert ((err == 0) || (err == controlPropertyNotFoundErr));
   
   // this_ptr might be null
   
   return this_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  WindowImpl::update ()
{
   if (_window_ref != 0)
   {
      OSStatus err = 0;
      
      /*err = HIWindowChangeClass (
         _window_ref, _window_class
      );
      assert (err == 0);*/
      
      /*WindowGroupRef window_group_ref = GetWindowGroupOfClass (_group_window_class);
      assert (window_group_ref != 0);
      
      err = SetWindowGroup (
         _window_ref, window_group_ref
      );
      assert (err == 0);
      
      err = ChangeWindowAttributes (
         _window_ref, _window_attributes, kWindowNoAttributes
      );
      assert (err == 0);*/
      
      if (_window_class == kSimpleWindowClass)
      {
         ChangeWindowAttributes (
            _window_ref, kWindowNoTitleBarAttribute, kWindowNoAttributes
         );
      }
      else
      {
         ChangeWindowAttributes (
            _window_ref, kWindowNoAttributes, kWindowNoTitleBarAttribute
         );
      }
   }
}


   
/*
==============================================================================
Name : handle_event_proc
==============================================================================
*/

OSStatus WindowImpl::handle_event_proc (EventHandlerCallRef /*handler_call_ref*/, EventRef event_ref, void * user_data)
{
   OSStatus ret_val = noErr;
   
   try
   {
      WindowImpl * this_ptr = (WindowImpl *) user_data;
      
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

OSStatus WindowImpl::handle_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventClass (event_ref))
   {
   case kEventClassMouse:
      ret_val = handle_mouse_event (event_ref);
      break;

   case kEventClassWindow:
      ret_val = handle_window_event (event_ref);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_mouse_event
Description :
   Dispatch mouse event to views, that are not normally automatically
   dispatched to views.
   The mouse is always captured, like on Windows.
   
   Event propagation on MacOS X
   kEventMouseDown & kEventMouseMoved are only sent when the mouse is
   over the window structure (including decoration)
   kEventMouseDragged & kEventMouseUp are sent even when the mouse is
   outside the window structure.
==============================================================================
*/

OSStatus WindowImpl::handle_mouse_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   if (GetEventKind (event_ref) == kEventMouseDown)
   {
      HIViewRef view_ref = 0;

      HIViewGetViewForMouseEvent (HIViewGetRoot (_window_ref), event_ref, &view_ref);
      
      if (view_ref != 0)
      {
         ViewImpl * view_ptr = ViewImpl::get (view_ref);
         
         if (view_ptr != 0)
         {
            _target_view_ptr = view_ptr;
         }
      }
   }
   
   if (GetEventKind (event_ref) == kEventMouseMoved)
   {
      HIViewRef view_ref = 0;

      HIViewGetViewForMouseEvent (HIViewGetRoot (_window_ref), event_ref, &view_ref);
      
      if (view_ref != 0)
      {
         ViewImpl * view_ptr = ViewImpl::get (view_ref);
         
         if (view_ptr != 0)
         {
            ret_val = view_ptr->handle_mouse_event (event_ref);
         }
      }
   }
   else if (_target_view_ptr != 0)
   {
      ret_val = _target_view_ptr->handle_mouse_event (event_ref);
   }
   
   if (GetEventKind (event_ref) == kEventMouseUp)
   {
      _target_view_ptr = 0;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_window_event
==============================================================================
*/

OSStatus WindowImpl::handle_window_event (EventRef event_ref)
{
   OSStatus ret_val = noErr;
   
   switch (GetEventKind (event_ref))
   {
   case kEventWindowBoundsChanged:
      _window.window_bounds_changed ();
      break;

   case kEventWindowClose:
      bool close_flag = _window.req_close ();
      
      if (close_flag)
      {
         // will close the window
         ret_val = eventNotHandledErr;
         
         if (_window_event_handler_ref != 0)
         {
            OSStatus err = RemoveEventHandler (_window_event_handler_ref);
            assert (err == 0);
            _window_event_handler_ref = 0;
         };
         
         // standard window handler is going to dispose the window
         _window_ref = 0;
      }
      break;
   }
      
   return ret_val;
}



PropertyCreator   WindowImpl::_property_creator = 'OmFo';
PropertyTag       WindowImpl::_property_tag = 'this';



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
