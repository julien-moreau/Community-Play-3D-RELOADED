/*****************************************************************************

        WindowImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71407

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

#include "ohm/math/fnc.h"
#include "ohm/opal/MenuStdId.h"
#include "ohm/opal/View.h"
#include "ohm/opal/View.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/win32/ApplicationImpl.h"
#include "ohm/opal/win32/MenuBarImpl.h"
#include "ohm/opal/win32/ScreensImpl.h"
#include "ohm/opal/win32/WindowImpl.h"
#include "ohm/sys/win32/ExceptionHandler.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/txt/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Creates a window with dummy rects, and keep it hidded.
Throws : Nothing
==============================================================================
*/

WindowImpl::WindowImpl (Window & window)
:  _window (window)
,  _min_bounds (opa::Size::_zero)
,  _max_bounds (opa::Size::_infinity)
,  _style (0)
,  _style_ex (0)
,  _hwnd (0)
,  _hwnd_owner (0)
,  _menu_owner_flag (false)
,  _modified_flag (false)
,  _title (L"Untitled")
,  _observer ("opal::WindowImpl")
,  _last_focus_ptr (0)
{
   
   
   int   err = 0;
   
   if (! _registered_flag && _window_cnt == 0)
   {
      WNDCLASSEXW wcex;

      wcex.cbSize          = sizeof (wcex); 
      wcex.style           = CS_DBLCLKS;
      wcex.lpfnWndProc     = handle_event_proc;
      wcex.cbClsExtra      = 0;
      wcex.cbWndExtra      = 0;
      wcex.hInstance       = ApplicationImpl::use_instance ().get_hinst ();
      wcex.hIcon           = 0;
      wcex.hCursor         = ::LoadCursor (NULL, IDC_ARROW);
      wcex.hbrBackground   = ::HBRUSH (COLOR_APPWORKSPACE + 1);
      wcex.lpszMenuName    = 0;
      wcex.lpszClassName   = _class_name_0;
      wcex.hIconSm         = 0;

      if (::RegisterClassExW (&wcex) == 0)
      {
         assert (false);
         err = -1;
      }
   }
   
   ::SetWindowLongPtr (_hwnd, GWLP_USERDATA, reinterpret_cast <::LONG_PTR> (this));

   _observer.bind_process <
      WindowImpl, 
      &WindowImpl::process_event
   > (*this);

   task::RunLoopGui::use_instance ().add (_observer);
}



/*
==============================================================================
Name : dtor
Throws : Nothing
==============================================================================
*/

WindowImpl::~WindowImpl ()
{
   try
   {
      task::RunLoopGui::use_instance ().remove (_observer);

      if (_menu_owner_flag)
      {
         MenuBarImpl::use_instance ().unbind (_hwnd);
         _menu_owner_flag = false;
      }
      if (_hwnd != 0)
      {
         ::DestroyWindow (_hwnd);
         _hwnd = 0;
         --_window_cnt;
      }
      
      if (_registered_flag && _window_cnt == 0)
      {
         if (::UnregisterClassW (_class_name_0, ApplicationImpl::use_instance ().get_hinst ()) != 0)
         {
            _registered_flag = false;
         }
         else
         {
            assert (false);
         }
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_class_document
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_class_document ()
{
   if (_hwnd == 0)
   {
      _style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
      _style_ex = WS_EX_OVERLAPPEDWINDOW;
   }
   else
   {
      // window is already created : mutate appearance
      
      ::LONG style = GetWindowLong (_hwnd, GWL_STYLE);
      
      style |= WS_OVERLAPPED;
      style |= WS_CAPTION;
      style |= WS_SYSMENU;
      style |= WS_THICKFRAME;
      
      _style_ex = WS_EX_OVERLAPPEDWINDOW;
      
      ::SetWindowLong (_hwnd, GWL_STYLE, style);
      ::SetWindowLong (_hwnd, GWL_EXSTYLE, _style_ex);
      ::SetWindowPos (
         _hwnd, 0, 0, 0, 0, 0,
         SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
      );
   }
}



void  WindowImpl::set_class_owned (archi::Int64 owner_window_data)
{
   assert (owner_window_data != 0);

   _hwnd_owner = reinterpret_cast <::HWND> (owner_window_data);

   _style |= WS_CAPTION | WS_SYSMENU;

   // WS_EX_PALETTEWINDOW without WS_EX_TOPMOST
   _style_ex = WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW;

}



/*
==============================================================================
Name : set_class_floating
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_class_floating ()
{
   _style |= WS_CAPTION | WS_SYSMENU;
   _style_ex = WS_EX_PALETTEWINDOW;
}



/*
==============================================================================
Name : set_class_no_decoration
==============================================================================
*/

void  WindowImpl::set_class_no_decoration ()
{
   if (_hwnd == 0)
   {
      _style_ex = 0;
   }
   else
   {
      // window is already created : mutate appearance
      
      ::LONG style = ::GetWindowLong (_hwnd, GWL_STYLE);
      style &= ~ WS_CAPTION;
      style &= ~ WS_THICKFRAME;
      
      _style_ex = 0;
      
      ::SetWindowLong (_hwnd, GWL_STYLE, style);
      ::SetWindowLong (_hwnd, GWL_EXSTYLE, _style_ex);
      ::SetWindowPos (
         _hwnd, 0, 0, 0, 0, 0,
         SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
      );
   }
}



/*
==============================================================================
Name : set_class_floating_no_decoration
==============================================================================
*/

void  WindowImpl::set_class_floating_no_decoration ()
{
   // Is this enough?
   _style_ex = WS_EX_PALETTEWINDOW;
}



/*
==============================================================================
Name : set_class_floating_overlay_group
==============================================================================
*/

void  WindowImpl::set_class_floating_overlay_group ()
{
   // Is this enough?
   _style |= WS_CAPTION | WS_SYSMENU;
   _style_ex = WS_EX_PALETTEWINDOW;
}



/*
==============================================================================
Name : set_class_dialog
==============================================================================
*/

void  WindowImpl::set_class_dialog ()
{
   _style_ex = WS_EX_DLGMODALFRAME;
}



/*
==============================================================================
Name : set_class_overlay
==============================================================================
*/

void  WindowImpl::set_class_overlay ()
{
   _style_ex = 0;
}



/*
==============================================================================
Name : set_overlay_group
==============================================================================
*/

void  WindowImpl::set_overlay_group ()
{
   _style_ex = 0;
}



/*
==============================================================================
Name : set_no_shadow
==============================================================================
*/

void  WindowImpl::set_no_shadow ()
{
}



/*
==============================================================================
Name : set_properties
==============================================================================
*/

void  WindowImpl::set_properties (bool close_box_flag, bool zoom_box_flag, bool collapse_box_flag, bool resizable_flag)
{
   // DWORD _style
   
   if (close_box_flag)
   {
      // they always have close boxes
   }
   
   if (zoom_box_flag)
   {
      _style |= WS_MAXIMIZEBOX;
   }
   
   if (collapse_box_flag)
   {
      _style |= WS_MINIMIZEBOX;
   }
   
   if (resizable_flag)
   {
      _style |= WS_THICKFRAME;
   }
}



/*
==============================================================================
Name : is_class_document
==============================================================================
*/

bool  WindowImpl::is_class_document () const
{
   return (_style_ex == WS_EX_OVERLAPPEDWINDOW);
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
Throws : Nothing
==============================================================================
*/

void  WindowImpl::create ()
{
   /* MSDN - WS_CLIPCHILDREN :
      Excludes the area occupied by child windows when drawing within
      the parent window. Used when creating the parent window
   */

   _style |= WS_CLIPCHILDREN;
   
   _registered_flag = true;
   ++ _window_cnt;
   _hwnd = ::CreateWindowExW (
      _style_ex,
      _class_name_0,
      _title.c_str (),
      _style,
      0, 0,
      CW_USEDEFAULT, CW_USEDEFAULT,
      _hwnd_owner,   // parent/owner window
      0,
      ApplicationImpl::use_instance ().get_hinst (),
      0
   );
   assert (_hwnd != 0);
   
   ::SetWindowLongPtr (_hwnd, GWLP_USERDATA, reinterpret_cast <::LONG_PTR> (this));
   
   if ((_style & WS_CAPTION) == 0)
   {
      ::LONG style = GetWindowLong (_hwnd, GWL_STYLE);
      style &= ~ WS_CAPTION;
      
      ::SetWindowLong (_hwnd, GWL_STYLE, style);
      ::SetWindowPos (
         _hwnd, 0, 0, 0, 0, 0,
         SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
      );
   }
   
   update_bounds ();
}



/*
==============================================================================
Name : show
Throws : Nothing
==============================================================================
*/

void  WindowImpl::show ()
{
   assert (_hwnd != 0);
   
   ::ShowWindow (_hwnd, SW_SHOW);
}



/*
==============================================================================
Name : hide
Throws : Nothing
==============================================================================
*/

void  WindowImpl::hide ()
{
   assert (_hwnd != 0);

   ::ShowWindow (_hwnd, SW_HIDE);
}



/*
==============================================================================
Name : collapse
Throws : Nothing
==============================================================================
*/

void  WindowImpl::collapse ()
{
   assert (_hwnd != 0);
   
   ::ShowWindow (_hwnd, SW_MINIMIZE);
}



/*
==============================================================================
Name : set_origin
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_origin (const opa::Point & origin)
{
   assert (_hwnd != 0);
   
   RECT        old_rect;
   ::GetWindowRect (_hwnd, &old_rect);
   BOOL ok_flag = ::MoveWindow (
      _hwnd,
      int (origin._x), int (origin._y),
      old_rect.right - old_rect.left,
      old_rect.bottom - old_rect.top,
      TRUE
   );
   assert (ok_flag);
}



/*
==============================================================================
Name : get_origin
Throws : Nothing
==============================================================================
*/

opa::Point  WindowImpl::get_origin ()
{
   assert (_hwnd != 0);
   
   ::RECT         old_rect;
   ::GetWindowRect (_hwnd, &old_rect);
   
   opa::Point ret_val (float (old_rect.left), float (old_rect.top));
   
   return (ret_val);
}



/*
==============================================================================
Name : get_content_origin
Throws : Nothing
==============================================================================
*/

opa::Point  WindowImpl::get_content_origin ()
{
   assert (_hwnd != 0);
   
   ::POINT        old_pt;
   old_pt.x = 0;
   old_pt.y = 0;
   ::ClientToScreen (_hwnd, &old_pt);
   
   opa::Point ret_val (float (old_pt.x), float (old_pt.y));
   
   return (ret_val);
}



/*
==============================================================================
Name : reposition_center
Throws : Nothing
==============================================================================
*/

void  WindowImpl::reposition_center (Window * parent_window_ptr, bool screen_relative_flag)
{
   assert (_hwnd != 0);
   
   if (parent_window_ptr != 0)
   {
      ::HWND parent_hwnd = parent_window_ptr->get_system_window ();
      
      if (screen_relative_flag)
      {
         int screen_width  = ::GetSystemMetrics (SM_CXFULLSCREEN);
         int screen_height = ::GetSystemMetrics (SM_CYFULLSCREEN);
         
         ::RECT   old_rect;
         ::GetWindowRect (_hwnd, &old_rect);
         
         ::BOOL ok_flag = ::MoveWindow (
            _hwnd,
            screen_width / 2 - (old_rect.right - old_rect.left) / 2,
            screen_height / 2 - (old_rect.bottom - old_rect.top) / 2,
            old_rect.right - old_rect.left,
            old_rect.bottom - old_rect.top,
            TRUE
         );
         assert (ok_flag);
      }
      else
      {
         ::RECT   parent_rect;
         ::GetWindowRect (_hwnd, &parent_rect);

         ::RECT   old_rect;
         ::GetWindowRect (_hwnd, &old_rect);
         
         ::BOOL ok_flag = ::MoveWindow (
            _hwnd,
            (parent_rect.left + parent_rect.right) / 2 - (old_rect.right - old_rect.left) / 2,
            (parent_rect.top + parent_rect.bottom) / 2 - (old_rect.bottom - old_rect.top) / 2,
            old_rect.right - old_rect.left,
            old_rect.bottom - old_rect.top,
            TRUE
         );
         assert (ok_flag);
      }
   }
   else
   {
      int screen_width  = ::GetSystemMetrics (SM_CXSCREEN);
      int screen_height = ::GetSystemMetrics (SM_CYSCREEN);
      
      ::RECT   old_rect;
      ::GetWindowRect (_hwnd, &old_rect);
      
      ::BOOL ok_flag = ::MoveWindow (
         _hwnd,
         screen_width / 2 - (old_rect.right - old_rect.left) / 2,
         screen_height / 2 - (old_rect.bottom - old_rect.top) / 2,
         old_rect.right - old_rect.left,
         old_rect.bottom - old_rect.top,
         TRUE
      );
      assert (ok_flag);
   }
}



/*
==============================================================================
Name : reposition_alert
Throws : Nothing
==============================================================================
*/

void  WindowImpl::reposition_alert (Window * parent_window_ptr, bool screen_relative_flag)
{
   assert (_hwnd != 0);
   
   if (parent_window_ptr != 0)
   {
      ::HWND parent_hwnd = parent_window_ptr->get_system_window ();
      
      if (screen_relative_flag)
      {
         int screen_width  = ::GetSystemMetrics (SM_CXSCREEN);
         int screen_height = ::GetSystemMetrics (SM_CYSCREEN);
         
         ::RECT   old_rect;
         ::GetWindowRect (_hwnd, &old_rect);
         
         ::BOOL ok_flag = ::MoveWindow (
            _hwnd,
            screen_width / 2 - (old_rect.right - old_rect.left) / 2,
            screen_height / 3 - (old_rect.bottom - old_rect.top) / 2,
            old_rect.right - old_rect.left,
            old_rect.bottom - old_rect.top,
            TRUE
         );
         assert (ok_flag);
      }
      else
      {
         ::RECT   parent_rect;
         ::GetWindowRect (_hwnd, &parent_rect);

         ::RECT   old_rect;
         ::GetWindowRect (_hwnd, &old_rect);
         
         ::BOOL ok_flag = ::MoveWindow (
            _hwnd,
            (parent_rect.left + parent_rect.right) / 2 - (old_rect.right - old_rect.left) / 2,
            parent_rect.top + (parent_rect.bottom - parent_rect.top) / 3 - (old_rect.bottom - old_rect.top) / 2,
            old_rect.right - old_rect.left,
            old_rect.bottom - old_rect.top,
            TRUE
         );
         assert (ok_flag);
      }
   }
   else
   {
      int screen_width  = ::GetSystemMetrics (SM_CXSCREEN);
      int screen_height = ::GetSystemMetrics (SM_CYSCREEN);
      
      ::RECT   old_rect;
      ::GetWindowRect (_hwnd, &old_rect);
      
      BOOL ok_flag = ::MoveWindow (
         _hwnd,
         screen_width / 2 - (old_rect.right - old_rect.left) / 2,
         screen_height / 3 - (old_rect.bottom - old_rect.top) / 2,
         old_rect.right - old_rect.left,
         old_rect.bottom - old_rect.top,
         TRUE
      );
      assert (ok_flag);
   }
}



/*
==============================================================================
Name : resize_default_main_screen
Throws : Nothing
==============================================================================
*/

void  WindowImpl::resize_default_main_screen ()
{
   assert (_hwnd != 0);
   
   opa::Rect rect = ScreensImpl::get_main_screen_without_decorations ();
   
   ::BOOL ok_flag = ::MoveWindow (
      _hwnd,
      int (rect._origin._x), int (rect._origin._y),
      int (rect._size._width), int (rect._size._height),
      TRUE
   );
   assert (ok_flag);
}



/*
==============================================================================
Name : set_bounds
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_bounds (const opa::Size & bounds)
{
   assert (_hwnd != 0);
   
   ::RECT         win_rect;
   ::RECT         old_rect;
   ::BOOL         res_flag;
   
   res_flag = ::SetRect (&win_rect, 0, 0, int (bounds._width), int (bounds._height));
   assert (res_flag != 0);

   res_flag = ::AdjustWindowRectEx (
      &win_rect,
      ::GetWindowLong (_hwnd, GWL_STYLE),
      ::GetMenu (_hwnd) != 0,
      ::GetWindowLong (_hwnd, GWL_EXSTYLE)
   );
   assert (res_flag != 0);

   res_flag = ::GetWindowRect (_hwnd, &old_rect);
   assert (res_flag != 0);

   res_flag = ::MoveWindow (
      _hwnd,
      old_rect.left,
      old_rect.top,
      win_rect.right - win_rect.left,
      win_rect.bottom - win_rect.top,
      TRUE
   );
   assert (res_flag != 0);
}



/*
==============================================================================
Name : get_bounds
Throws : Nothing
==============================================================================
*/

opa::Size   WindowImpl::get_bounds ()
{
   return _cur_bounds;
}



/*
==============================================================================
Name : set_limit_bounds
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_limit_bounds (const opa::Size & min_bounds, const opa::Size & max_bounds)
{
   assert (_hwnd != 0);
   
   _min_bounds = min_bounds;
   _max_bounds = max_bounds;
}



/*
==============================================================================
Name : is_active
Throws : Nothing
==============================================================================
*/

bool  WindowImpl::is_active ()
{
   // we will use a notification scheme soon anyway
   
   return true;
}



/*
==============================================================================
Name : set_title
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_title (const std::string & title)
{
   assert (_hwnd != 0);

   txt::String16  title_utf16;
   const int      ret_val = txt::conv_utf8_to_utf16 (title_utf16, title.c_str ());
   if (ret_val != txt::Err_OK)
   {
      ohm_util_TRACE_ERROR (
         "set_title(): cannot convert the window title to UTF-16."
      );
   }
   else
   {
      _title = title_utf16;
      update_title ();
   }
}



/*
==============================================================================
Name : set_alpha
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_alpha (float alpha)
{
   assert (_hwnd != 0);
   
   
   // see : http://msdn.microsoft.com/en-us/library/ms632599(VS.85).aspx
}



/*
==============================================================================
Name : set_modified
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_modified (bool modified_flag)
{
   assert (_hwnd != 0);

   _modified_flag = modified_flag;
   update_title ();
}



/*
==============================================================================
Name : set_proxy_path
Throws : Nothing
==============================================================================
*/

void  WindowImpl::set_proxy_path (const std::string & proxy_path)
{
   // nothing
}



/*
==============================================================================
Name : window_bounds_changed
Description :
   Notify all child views of window bounds change.
Throws : Nothing
==============================================================================
*/

void  WindowImpl::window_bounds_changed ()
{
   assert (_hwnd != 0);
   
   ViewSet::iterator it = _view_ptr_arr.begin ();
   const ViewSet::iterator it_end = _view_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      View * view_ptr = *it;
      
      view_ptr->window_layout_changed ();
   }
}



/*
==============================================================================
Name : get_system_window
Throws : Nothing
==============================================================================
*/

::HWND   WindowImpl::get_system_window () const
{
   assert (_hwnd != 0);
   
   return _hwnd;
}



/*
==============================================================================
Name : get_window
Throws : Nothing
==============================================================================
*/

Window & WindowImpl::get_window ()
{
   return _window;
}



/*
==============================================================================
Name : get_window
Throws : Nothing
==============================================================================
*/

void  WindowImpl::bind_menu_bar ()
{
   MenuBarImpl::use_instance ().bind (_hwnd);
   _menu_owner_flag = true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const txt::Char16 WindowImpl::_class_name_0 [] = L"Ohm_Force_Opal_WindowImpl_Class_Name";
long  WindowImpl::_window_cnt = 0;
bool  WindowImpl::_registered_flag = false;



/*
==============================================================================
Name : handle_event_proc
Throws : Nothing
==============================================================================
*/

LRESULT CALLBACK  WindowImpl::handle_event_proc (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   LRESULT ret_val = 0;
   
   __try
   {
      WindowImpl *      obj_ptr = reinterpret_cast <WindowImpl *> (
         ::GetWindowLongPtrW (hwnd, GWLP_USERDATA)
      );
      
      if (obj_ptr == 0)
      {
         ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
      }
      else
      {
         ret_val = obj_ptr->handle_event (hwnd, message, wparam, lparam);
      }
   }
   __except (sys::win32::ExceptionHandler::exec (GetExceptionInformation ()))
   {
      assert (false);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_event
Description : handle the Windows event. If the RunLoopGui is not processing
   events, there is reetrency, so we defer the processing of the message.
   All message currently processed require the proc to return 0, but that
   may change in the future
==============================================================================
*/

LRESULT  WindowImpl::handle_event (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   LRESULT ret_val = 0;

   switch (message)
   {
   case WM_ACTIVATEAPP:
      {
         task::RunLoopGui &   rlg = task::RunLoopGui::use_instance ();
         if (! rlg.is_processing_event ())
         {
            rlg.notify_pre_gui_event ();
            ret_val = handle_event_actual (false, hwnd, message, wparam, lparam);
            rlg.notify_post_gui_event ();
         }
         else
         {
            // defer message processing
            task::Event & event = ohm_task_EVENT_GET;
            event.push (hwnd);
            event.push (message);
            event.push (wparam);
            event.push (lparam);

            _observer.send (event);

            ret_val = (message == WM_SIZING) ? TRUE : 0; // ask Bill why WM_SIZING is an exception 
            //#* LDS: WM_SIZING ??? assert (! computer_on_fire ());
         }
      }
      break;

   case WM_SIZE:
      update_bounds ();
      window_bounds_changed ();
      break;
      
   case WM_SIZING:
      {
         ::RECT *       rect_ptr = reinterpret_cast <::RECT *> (lparam);
         
         if (rect_ptr->right - rect_ptr->left < _min_bounds._width)
         {
            rect_ptr->right = rect_ptr->left + math::round_int (_min_bounds._width);
            ret_val = TRUE;
         }
      
         if (rect_ptr->right - rect_ptr->left > _max_bounds._width)
         {
            rect_ptr->right = rect_ptr->left + math::round_int (_max_bounds._width);
            ret_val = TRUE;
         }
         
         if (rect_ptr->bottom - rect_ptr->top < _min_bounds._height)
         {
            rect_ptr->bottom = rect_ptr->top + math::round_int (_min_bounds._height);
            ret_val = TRUE;
         }
         
         if (rect_ptr->bottom - rect_ptr->top > _max_bounds._height)
         {
            rect_ptr->bottom = rect_ptr->top + math::round_int (_max_bounds._height);
            ret_val = TRUE;
         }
         
         update_bounds ();
         window_bounds_changed ();
      
         if (ret_val == 0)
         {
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
      }
      break;
   
   case WM_ACTIVATE:
      {
         if (
               (LOWORD (wparam) != WA_INACTIVE) // we are being activated
            && (HIWORD (wparam) == 0)           // we are not minimized
            && !_view_ptr_arr.empty ()
         )
         {
            // a voir avec RDI comment c'est fait sous mac
            /*assert (_view_ptr_arr.size () == 1);
            ViewSet::iterator it = _view_ptr_arr.begin ();
            ::SetFocus ((*it)->get_system_view ());
            ::SetActiveWindow ((*it)->get_system_view ());*/

            if (   _last_focus_ptr != 0
                && _view_ptr_arr.find (_last_focus_ptr) != _view_ptr_arr.end ())
            {
               _last_focus_ptr->use_impl ().acquire_keyboard_focus ();
            }
         }
      }
      break;

   case WM_CLOSE:
      {
         _window.req_close ();
      }
      break;
      
   case WM_COMMAND:
      {
         if (lparam == 0)
         {
            MenuBarImpl::MenuId menu_id = LOWORD (wparam);
            ::WORD source_val = HIWORD (wparam);   // 0: menu, 1: shortcut, >1: control
            assert (source_val >= 0);
            assert (source_val <= 1);

            bool shortcut_flag = (source_val == 1);

            if (MenuBarImpl::use_instance ().handle (menu_id, shortcut_flag))
            {
               ret_val = TRUE;
            }
         }

         if (ret_val == 0)
         {
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
      }
      break;

      case WM_COPY:
      {
         const opal::MenuStdId menu_id = opal::MenuStdId_COPY;
         const bool shortcut_flag = true;

         if (MenuBarImpl::use_instance ().handle (menu_id, shortcut_flag))
         {
            ret_val = TRUE;
         }

         if (ret_val == 0)
         {
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
      }
      break;

      case WM_CUT:
      {
         const opal::MenuStdId menu_id = opal::MenuStdId_CUT;
         const bool shortcut_flag = true;

         if (MenuBarImpl::use_instance ().handle (menu_id, shortcut_flag))
         {
            ret_val = TRUE;
         }

         if (ret_val == 0)
         {
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
      }
      break;

      case WM_PASTE:
      {
         const opal::MenuStdId menu_id = opal::MenuStdId_PASTE;
         const bool shortcut_flag = true;

         if (MenuBarImpl::use_instance ().handle (menu_id, shortcut_flag))
         {
            ret_val = TRUE;
         }

         if (ret_val == 0)
         {
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
      }
      break;

   /* case WM_MENUCOMMAND:
      {
         ::HMENU  hmenu = ::HMENU (lparam);
         ::UINT   item_index = wparam;

         ::MENUITEMINFO item_info;
         ::memset (&item_info, 0, sizeof (item_info));

         item_info.cbSize = sizeof (item_info);
         item_info.fMask = MIIM_DATA;
         
         ret_val = ::GetMenuItemInfo (hmenu, item_index, TRUE, &item_info);
         
         if (ret_val != 0)
         {
            opal::MenuItem * item_ptr = (MenuItem *) (item_info.dwItemData);

            MenuEvent menu_event;
            menu_event._menu_item_ptr = item_ptr;
            item_ptr->get_delegate ().signal (menu_event);
         }

         if (ret_val == 0)
         {
            // either unknown menu or accelerator or control-defined notification code (none yet)
            assert (false);
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
      }
      break;
*/

/* case WM_ERASEBKGND:
*/

   case WM_ERASEBKGND:
      // block the event
      ret_val = 0;
      break;

   default:
      ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
      break;
   }

   return ret_val;
}



/*
==============================================================================
Name : handle_event
Throws : Nothing
==============================================================================
*/

LRESULT  WindowImpl::handle_event_actual (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   LRESULT ret_val = 0;
   
   switch (message)
   {

   case WM_ACTIVATEAPP:
      {
         if (wparam == FALSE) // deactivation
         {
            assert (task::RunLoopGui::use_instance ().is_processing_event ());

            std::set <View *>::iterator it = _view_ptr_arr.begin ();
            const std::set <View *>::iterator it_end = _view_ptr_arr.end ();
            task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
            
            for ( ; it != it_end ; ++it)
            {
               (*it)->user_input_lost ();
            }
         }
      }
      break;

   default:
      assert (false);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : add_view
Throws : Nothing
==============================================================================
*/

void  WindowImpl::add_view (View * view_ptr)
{
   assert (_view_ptr_arr.find (view_ptr) == _view_ptr_arr.end ());
   
   _view_ptr_arr.insert (view_ptr);
}



/*
==============================================================================
Name : remove_view
Throws : Nothing
==============================================================================
*/

void  WindowImpl::remove_view (View * view_ptr)
{  
   assert (_view_ptr_arr.find (view_ptr) != _view_ptr_arr.end ());
   
   _view_ptr_arr.erase (view_ptr);
   if (_last_focus_ptr == view_ptr)
   {
      _last_focus_ptr = 0;
   }
}



void  WindowImpl::update_bounds ()
{
   assert (_hwnd != 0);
   
   ::RECT         win_rect;
   ::BOOL         err_val;

   // new 
   err_val = ::GetClientRect (_hwnd, &win_rect);
   assert (err_val != 0);

   opa::Size      bounds (
      float (win_rect.right - win_rect.left),
      float (win_rect.bottom - win_rect.top)
   );

   _cur_bounds = bounds;
}



void  WindowImpl::update_title ()
{
   txt::String16  title_mod = _title;

   if (_modified_flag)
   {
      title_mod = L"* " + title_mod;
   }

   const ::BOOL   ok_flag = ::SetWindowTextW (_hwnd, title_mod.c_str ());
   assert (ok_flag);
}



void  WindowImpl::process_event (task::Event & event)
{
   ::HWND         hwnd   = event.pop <::HWND> ();
   ::UINT         msg    = event.pop <::UINT> ();
   ::WPARAM       wparam = event.pop <::WPARAM> ();
   ::LPARAM       lparam = event.pop <::LPARAM> ();

   handle_event_actual (true, hwnd, msg, wparam, lparam);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
