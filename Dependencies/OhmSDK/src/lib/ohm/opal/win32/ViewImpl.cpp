/*****************************************************************************

        ViewImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70966

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

#include "ohm/lang/fnc.h"
#include "ohm/math/fnc.h"
#include "ohm/opal/win32/ViewImpl.h"
#include "ohm/opal/win32/WindowImpl.h"
#include "ohm/opal/AutoContextState.h"
#include "ohm/opal/View.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/opal/Key.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/win32/ApplicationImpl.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/directx/DirectXContext.h"
#include "ohm/sys/win32/ExceptionHandler.h"
#include "ohm/sys/win32/LocalUseOle.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/out/Print.h"
#include "ohm/txt/utf8/utf8.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/fnc.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/win32/RunLoopGuiImpl.h"
#include "ohm/util/trace.h"

#include <ole2.h>
#include <shlwapi.h>
#include <wininet.h>

#include <cassert>



namespace ohm
{
namespace opal
{


/*
   Keyboard input
   http://msdn.microsoft.com/en-us/library/ms646267(v=vs.85).aspx
*/


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ViewImpl::ViewImpl (Window & window, View & view)
:  _window (window)
,  _view (view)
,  _hwnd (0)
,  _mouse_position (-1, -1)
,  _mouse_click_position (-1, -1)
,  _mouse_in_flag (false)
,  _relative_hidden_flag (false)
,  _block_flag (false)
,  _impl (*this)
,  _key_catched_flag (false)
,  _mouse_state (0)
,  _system_mouse_capture_flag (false)
,  _observer ("opal::ViewImpl")
,  _ole_session ()
,  _ole_ref_count (0)
{
   for (int i = 0 ; i < MButBit_NBR_ELT ; ++i)
   {
      _click_cnt_arr [i] = 0;
      _click_time_s_arr [i] = 0.0;
   }
   
   int   err = 0;
   
   if (! _registered_flag && _window_cnt == 0)
   {
      ::WNDCLASSEXW  wcex;

      wcex.cbSize          = sizeof (wcex); 
      wcex.style           = CS_DBLCLKS;
      wcex.lpfnWndProc     = handle_event_proc;
      wcex.cbClsExtra      = 0;
      wcex.cbWndExtra      = 0;
      wcex.hInstance       = ApplicationImpl::use_instance ().get_hinst ();
      wcex.hIcon           = 0;
      wcex.hCursor         = 0;
      wcex.hbrBackground   = 0;
      wcex.lpszMenuName    = 0;
      wcex.lpszClassName   = _class_name_0;
      wcex.hIconSm         = 0;

      if (RegisterClassExW (&wcex) == 0)
      {
         assert (false);
         err = -1;
      }
   }
   
   ::HWND parent_hwnd = window.get_system_window ();
   
   // the child window cannot use CW_USEDEFAULT
   // or the resulting window will be 0x0
   
   opa::Size window_size = window.get_bounds ();
   
   /* MSDN - WS_CLIPSIBLINGS :
      Clips child windows relative to each other
   */

   ::DWORD style = 0;
   style |= WS_CHILD;
   style |= WS_CLIPSIBLINGS;
   
   ::DWORD style_ex = 0;
   
   _registered_flag = true;
   ++ _window_cnt;
   _hwnd = ::CreateWindowExW (
      style_ex,
      _class_name_0,
      L"",
      style,
      0, 0,
      int (window_size._width), int (window_size._height),
      parent_hwnd,   
      0,
      ApplicationImpl::use_instance ().get_hinst (),
      reinterpret_cast <::LPVOID> (this)  // Pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message
   );
   assert (_hwnd != 0);
   
   ::SetWindowLongPtr (_hwnd, GWLP_USERDATA, reinterpret_cast <::LONG_PTR> (this));
   
   _window.use_window_impl ().add_view (&_view);
   
   _observer.bind_process <
      ViewImpl, 
      &ViewImpl::process_event
   > (*this);

   task::RunLoopGui::use_instance ().add (_observer);

   assert (_view_map.find (_hwnd) == _view_map.end ());
   _view_map.insert (std::make_pair (_hwnd, this));
   
   // for now : take into account the last created view
   ::HWND hwnd = _window.get_system_window ();
   _view_map [hwnd] = this;
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
      assert (_view_map.find (_hwnd) != _view_map.end ());
      _view_map.erase (_view_map.find (_hwnd));

      ::HWND hwnd = _window.get_system_window ();
      std::map <::HWND, ViewImpl *>::iterator it = _view_map.find (hwnd);
      if (it != _view_map.end ())
      {
         _view_map.erase (it);
      }

      task::RunLoopGui::use_instance ().remove (_observer);

      _window.use_window_impl ().remove_view (&_view);

      if (_hwnd != 0)
      {
         KillTimer (_hwnd, 1);
         
         DestroyWindow (_hwnd);
         _hwnd = 0;
         --_window_cnt;

         assert (_ole_ref_count == 0);
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
Name : vt_init
Thread : Video
==============================================================================
*/

void  ViewImpl::vt_init ()
{
   _impl.vt_init ();
}



/*
==============================================================================
Name : vt_restore
Thread : Video
==============================================================================
*/

void  ViewImpl::vt_restore ()
{
   _impl.vt_restore ();
}



/*
==============================================================================
Name : vt_update
Thread : Video
==============================================================================
*/

void  ViewImpl::vt_update (const opa::Size & size)
{
   _impl.vt_update (size);
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
   _impl.vt_draw (wait_vbr_flag);
}



/*
==============================================================================
Name: key_pressed
==============================================================================
*/

bool ViewImpl::key_pressed (const opal::Key & key, bool repeated_flag)
{
#if defined ohm_opal_VIEW_USE_EVENT_MGR
   _sys_event._key = key;
   _sys_event._key_change = 
      repeated_flag ?
      SystemEvent::KeyChange_REPEATED :
      SystemEvent::KeyChange_PRESSED;
      
   const bool catched_flag = _view.handle_system_event (_sys_event);
   
   return catched_flag;
#else
   assert (false);
   return false;
#endif
}



/*
==============================================================================
Name: characters_typed
==============================================================================
*/

bool ViewImpl::characters_typed (const std::string & typed_utf8)
{
#if defined ohm_opal_VIEW_USE_EVENT_MGR
   _sys_event._char_utf8 = typed_utf8;
   
   const bool catched_flag = _view.handle_system_event (_sys_event);
   
   return catched_flag;
#else
   assert (false);
   return false;
#endif
}



/*
==============================================================================
Name : get_system_view
==============================================================================
*/

HWND  ViewImpl::get_system_view () const
{
   return _hwnd;
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
Name : set_origin
==============================================================================
*/

void  ViewImpl::set_origin (const opa::Point & point)
{
   assert (_hwnd != 0);

   ::RECT         old_rect;
   ::GetWindowRect (_hwnd, &old_rect);
   ::BOOL         ok_flag = ::MoveWindow (
      _hwnd,
      int (point._x), int (point._y),
      old_rect.right - old_rect.left,
      old_rect.bottom - old_rect.top,
      TRUE
   );
   assert (ok_flag);
}



/*
==============================================================================
Name : set_bounds
==============================================================================
*/

void  ViewImpl::set_bounds (const opa::Size & size)
{
   assert (_hwnd != 0);

   opa::Size old_size = get_bounds ();

   ::BOOL         ok_flag = ::MoveWindow (
      _hwnd,
      0, 0, // a view is for now always on 0, 0, as we managed only one view in a window
      int (size._width), int (size._height),
      TRUE
   );
   assert (ok_flag);
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

opa::Size   ViewImpl::get_bounds ()
{
   assert (_hwnd != 0);
   
   ::RECT         rect;
   ::GetWindowRect (_hwnd, &rect);
   
   const opa::Size      ret_val (
      float (rect.right - rect.left),
      float (rect.bottom - rect.top)
   );
   
   return ret_val;
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
   assert (_hwnd != 0);
   
   /*MSG msg;
   msg.hwnd = _hwnd;
   msg.message = WM_MOVE;
   msg.wParam = 0;
   RECT        rect;
   GetWindowRect (_hwnd, &rect);
   msg.lParam = MAKELPARAM (rect.left, rect.top);
   msg.time = timeGetTime ();
   POINT pt = {0, 0};
   msg.pt = pt;
   
   DispatchMessage (&msg);*/
}



/*
==============================================================================
Name : set_nbr_skip_frame
Description :
   Set the nbr of skipped frame to reduce GPU power consumption
==============================================================================
*/

void  ViewImpl::set_nbr_skip_frame (int nbr)
{
   // to do
   // _impl.set_nbr_skip_frame (nbr);
}



/*
==============================================================================
Name : show
==============================================================================
*/

void  ViewImpl::show ()
{
   assert (_hwnd != 0);
   
   ::ShowWindow (_hwnd, SW_SHOW);
}



/*
==============================================================================
Name : hide
==============================================================================
*/

void  ViewImpl::hide ()
{
   assert (_hwnd != 0);
   
   ::ShowWindow (_hwnd, SW_HIDE);
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  ViewImpl::invalidate (const opa::Rect & rect)
{
   _impl.invalidate (rect);   
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
   assert (_hwnd != 0);
   
   ::SetFocus (_hwnd);
   _window.use_window_impl ()._last_focus_ptr = &_view;
}


/*
==============================================================================
Name : release_keyboard_focus
==============================================================================
*/

void  ViewImpl::release_keyboard_focus ()
{
   assert (_hwnd != 0);
   
   ::SetFocus (0);
}



/*
==============================================================================
Name : notify_capture_changed
==============================================================================
*/

void  ViewImpl::notify_capture_changed ()
{
   _system_mouse_capture_flag = false;
}



/*
==============================================================================
Name : debug_toggle_show_invalid_rect
==============================================================================
*/

void  ViewImpl::debug_toggle_show_invalid_rect ()
{
   // to do (not mandatory, very low priority)
}



/*
==============================================================================
Name : get
Description :
   Returns the ViewImpl corresponding to the ViewImpl or WindowImpl hwnd.
==============================================================================
*/

ViewImpl * ViewImpl::get (::HWND hwnd)
{
   ViewImpl * obj_ptr = 0;
   
   std::map <::HWND, ViewImpl *>::iterator it = _view_map.find (hwnd);

   if (it != _view_map.end ())
   {
      obj_ptr = it->second;
   }

   return obj_ptr;
}



/*
==============================================================================
Name : DragEnter
==============================================================================
*/

::HRESULT   ViewImpl::DragEnter (::IDataObject * data_obj_ptr, ::DWORD key_state, ::POINTL location, ::DWORD * drop_effect_ptr)
{
   // we only accept file pathes (any file type)
   ::FORMATETC format;
   format.cfFormat = CF_HDROP;         // we just want a list of files dropped
   format.ptd = 0;                     // don't care the source
   format.dwAspect = DVASPECT_CONTENT; // interseted in the content
   format.lindex = -1;  
   format.tymed = TYMED_HGLOBAL;       // the file list is in memory
   
   if ((drop_effect_ptr == 0) || (data_obj_ptr == 0))
   {
      return E_UNEXPECTED;
   }

/* ::IEnumFORMATETC * enum_format_ptr = 0;
   
   ::HRESULT hresult = data_obj_ptr->EnumFormatEtc (::DATADIR_GET, &enum_format_ptr);

   if (hresult == S_OK)
   {
      assert (enum_format_ptr != 0);

      FORMATETC available_format;
      ::ULONG  fetch_count;

      hresult = enum_format_ptr->Next (1, &available_format, &fetch_count);

      while (hresult == S_OK)
      {
         // check the formats
         assert (fetch_count == 1);
         int coco = 2;
         hresult = enum_format_ptr->Next (1, &available_format, &fetch_count);
      }
   }
*/
   ::HRESULT hresult_query = data_obj_ptr->QueryGetData (&format);

   if (hresult_query != S_OK)
   {
      // don't accept the data
      *drop_effect_ptr = DROPEFFECT_NONE;
   }
   else
   {
      // whatever the modifiers, we only link, as we don't manage buffer moves yet
      *drop_effect_ptr = DROPEFFECT_LINK;
      acquire_keyboard_focus ();

      // bring the window to front

      ::SetWindowPos (_window.get_system_window (), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

      // fill opal::Data shared ptr

      ::STGMEDIUM stg_medium;

      hresult_query = data_obj_ptr->GetData (&format, &stg_medium);

      if (hresult_query == S_OK)
      {
         assert (stg_medium.tymed == TYMED_HGLOBAL);
         
         _drag_data_sptr = mem::SharedPtr <Data> (new Data ());
         
         fill_drag_data (*_drag_data_sptr, HDROP (stg_medium.hGlobal));

         // check how to release this thing
         // ::ReleaseStgMedium (&stg_medium);
         if (stg_medium.pUnkForRelease != 0)
         {
            stg_medium.pUnkForRelease->Release ();
         }
      }
   }

   return hresult_query;
}



/*
==============================================================================
Name : DragOver
==============================================================================
*/

::HRESULT   ViewImpl::DragOver (::DWORD key_state, ::POINTL location, ::DWORD * drop_effect_ptr)
{
   if (drop_effect_ptr == 0)
   {
      return E_UNEXPECTED;
   }
   
   // whatever the modifiers, we only copy, as we don't manage buffer moves yet
   *drop_effect_ptr = DROPEFFECT_COPY;

   // is it possible we are in a gui event at this point
   // we are interrupted
   // OLE messages are not sent through the view proc
   // we send this through the runloop

   ::POINT view_point;
   view_point.x = location.x;
   view_point.y = location.y;

   const BOOL ret_val = ::ScreenToClient (_hwnd, &view_point);
   assert (ret_val == TRUE);

   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_DRAG);
   event.push (opa::Point (float (view_point.x), float (view_point.y)));
   event.push (_drag_data_sptr);
      
   _observer.send (event);

   return S_OK;
}



/*
==============================================================================
Name : DragLeave
==============================================================================
*/

::HRESULT   ViewImpl::DragLeave ()
{
   _drag_data_sptr.swap (mem::SharedPtr <Data> ());

   return S_OK;
}



/*
==============================================================================
Name : Drop
==============================================================================
*/

::HRESULT   ViewImpl::Drop (::IDataObject * data_obj_ptr, ::DWORD key_state, ::POINTL location, ::DWORD *drop_effect_ptr)
{
   if ((drop_effect_ptr == 0) || (data_obj_ptr == 0))
   {
      return E_UNEXPECTED;
   }
   
   _drag_data_sptr.swap (mem::SharedPtr <Data> ());

   // we need to do the drop now!
   *drop_effect_ptr = DROPEFFECT_NONE;

   ::FORMATETC format;
   format.cfFormat = CF_HDROP;         // we just want a list of files dropped
   format.ptd = 0;                     // don't care the source
   format.dwAspect = DVASPECT_CONTENT; // interested in the content
   format.lindex = -1;  
   format.tymed = TYMED_HGLOBAL;       // the file list is in memory

   ::STGMEDIUM stg_medium;

   ::HRESULT hresult_query = data_obj_ptr->GetData (&format, &stg_medium);

   if (hresult_query == S_OK)
   {
      assert (stg_medium.tymed == TYMED_HGLOBAL);
      
      mem::SharedPtr <Data> data_sptr (new Data ());
      
      fill_drag_data (*data_sptr, HDROP (stg_medium.hGlobal));

      // point is in screen coordinate
      ::POINT view_point;
      view_point.x = location.x;
      view_point.y = location.y;

      const BOOL ret_val = ::ScreenToClient (_hwnd, &view_point);
      assert (ret_val == TRUE);

      // is it possible we are in a gui event at this point
      // we are interrupted
      // OLE messages are not sent through the view proc
      // we send this through the runloop

      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_DROP);
      event.push (opa::Point (float (view_point.x), float (view_point.y)));
      event.push (data_sptr);

      _observer.send (event);

      // check how to release this thing
      // ::ReleaseStgMedium (&stg_medium);
      if (stg_medium.pUnkForRelease != 0)
      {
         stg_medium.pUnkForRelease->Release ();
      }
   }

   return S_OK;
}



/*
==============================================================================
Name : QueryInterface
==============================================================================
*/

::HRESULT   ViewImpl::QueryInterface (REFIID riid, void ** interface_ptr_ptr)
{
   ::HRESULT hresult = E_POINTER;

   if (interface_ptr_ptr != 0)
   {
      if (riid == IID_IUnknown)
      {
         *interface_ptr_ptr = this;
         hresult = S_OK;
      }
      else if (riid == IID_IDropTarget)
      {
         *interface_ptr_ptr = this;
         hresult = S_OK;
      }
      else
      {
         *interface_ptr_ptr = 0;
         hresult = E_NOINTERFACE;
      }
   }

   return hresult;
}


/*
==============================================================================
Name : AddRef
==============================================================================
*/

::ULONG     ViewImpl::AddRef ()
{
   ++_ole_ref_count;

   return _ole_ref_count;
}



/*
==============================================================================
Name : Release
==============================================================================
*/

::ULONG     ViewImpl::Release ()
{
   --_ole_ref_count;
   assert (_ole_ref_count >= 0);

   return _ole_ref_count;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : handle_event_proc
==============================================================================
*/

::LRESULT CALLBACK   ViewImpl::handle_event_proc (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ::LRESULT      ret_val = 0;
   
   __try
   {
      //trace (hwnd, message, wparam, lparam);
      
      ViewImpl * obj_ptr = reinterpret_cast <ViewImpl *> (
         ::GetWindowLongPtr (hwnd, GWLP_USERDATA)
      );
      
      if (obj_ptr == 0)
      {
         if (message == WM_CREATE)
         {
            ::CREATESTRUCT * create_struct_ptr = reinterpret_cast <::CREATESTRUCT *> (lparam);
            
            ViewImpl * obj_ptr = reinterpret_cast <ViewImpl *> (
               create_struct_ptr->lpCreateParams
            );
            assert (obj_ptr != 0);

            ret_val = obj_ptr->handle_event (hwnd, message, wparam, lparam);
         }
         else
         {
            ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
         }
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

::LRESULT   ViewImpl::handle_event (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ::LRESULT      ret_val = TRUE;

   switch (message)
   {
   case WM_MOUSEMOVE:
   case WM_MOUSELEAVE:
   case WM_LBUTTONUP:
   case WM_MBUTTONUP:
   case WM_RBUTTONUP:
   case WM_LBUTTONDOWN:
   case WM_MBUTTONDOWN:
   case WM_RBUTTONDOWN:
   case WM_LBUTTONDBLCLK:
   case WM_MBUTTONDBLCLK:
   case WM_RBUTTONDBLCLK:
   case WM_MOUSEWHEEL:
   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
   case WM_KEYUP:
   case WM_SYSKEYUP:
   case WM_CHAR:
#if 0
   // For a Unicode window, this message is the same as WM_CHAR.
   // http://msdn.microsoft.com/en-us/library/dd374132(v=vs.85).aspx
   case WM_IME_CHAR:
#endif
   /*case WM_MOVE:
   case WM_MOVING:*/
   case  WM_CAPTURECHANGED:
      {
         ret_val = handle_event_auto (hwnd, message, wparam, lparam);
      }
      break;

   case WM_UNICHAR:
      if (wparam == UNICODE_NOCHAR)
      {
         // application process this message
         ret_val = TRUE;
      }
      else
      {
         ret_val = handle_event_auto (hwnd, message, wparam, lparam);
      }
      break;
   
   case WM_PAINT:
      {
         // identifical to the one returned by BeginPaint
         ::RECT dirty_rect;
         ::BOOL ok_flag = ::GetUpdateRect (hwnd, &dirty_rect, false);
         
         if (ok_flag)
         {
            opa::Rect rect;
            rect._origin._x = float (dirty_rect.left);
            rect._origin._y = float (dirty_rect.top);
            rect._size._width = float (dirty_rect.right - dirty_rect.left);
            rect._size._height = float (dirty_rect.bottom - dirty_rect.top);
            
            VideoThread::use_instance ().invalidate (_view, rect);
            
            ::ValidateRect (hwnd, &dirty_rect);
         }
      }
      break;
   
   case WM_ERASEBKGND:
      // block the event
      ret_val = 0;
      break;

   case WM_CREATE:
      {
         // register as a Drop Target
         ::HRESULT ole_err = ::RegisterDragDrop (hwnd, this);
         assert (ole_err == S_OK);

         if (ole_err != S_OK)
         {
            ohm_util_TRACE_ERROR1 ("::RegisterDragDrop failed with error %1%", ole_err);
         }
      }

      break;

   case WM_DESTROY:
      {
         // unregister as a Drop Target
         ::HRESULT ole_err = ::RevokeDragDrop (hwnd);
         assert (ole_err == S_OK);
      }
      break;

   default:
      ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
      break;
   }

   return ret_val;
}



/*
==============================================================================
Name : handle_event_auto
==============================================================================
*/

::LRESULT   ViewImpl::handle_event_auto (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ::LRESULT ret_val = TRUE;
   
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
      event.push (EventType_DEFER_MSG);
      event.push (hwnd);
      event.push (message);
      event.push (wparam);
      event.push (lparam);

      _observer.send (event);

      ret_val = 0;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_event_actual
==============================================================================
*/

::LRESULT   ViewImpl::handle_event_actual (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ::LRESULT      ret_val = TRUE;

   switch (message)
   {
   case WM_MOUSEMOVE:
      handle_mouse_move (lparam);
      ret_val = FALSE;
      break;

   case WM_MOUSELEAVE:
      handle_mouse_leave ();
      ret_val = FALSE;
      break;
   
   case WM_LBUTTONUP:
   case WM_MBUTTONUP:
   case WM_RBUTTONUP:
   case WM_LBUTTONDOWN:
   case WM_MBUTTONDOWN:
   case WM_RBUTTONDOWN:
   case WM_LBUTTONDBLCLK:
   case WM_MBUTTONDBLCLK:
   case WM_RBUTTONDBLCLK:
      handle_mouse_click (message, lparam);
      ret_val = FALSE;
      break;

   case WM_MOUSEWHEEL:
      handle_mouse_wheel (wparam);
      ret_val = FALSE;
      break;

   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
   case WM_KEYUP:
   case WM_SYSKEYUP:
      ret_val = handle_keyboard (deferred_flag, hwnd, message, wparam, lparam);
      break;
         
   case WM_CHAR:
#if 0
   // For a Unicode window, this message is the same as WM_CHAR.
   // http://msdn.microsoft.com/en-us/library/dd374132(v=vs.85).aspx
   // Note: we'd better use the Composition String from the Input Method Manager.
   // http://msdn.microsoft.com/en-us/library/dd317765(v=VS.85).aspx
   case WM_IME_CHAR:
#endif
   case WM_UNICHAR:
      ret_val = handle_character (deferred_flag, hwnd, message, wparam, lparam);
      break;
   
   case  WM_CAPTURECHANGED:
      if (_system_mouse_capture_flag)
      {
         _system_mouse_capture_flag = false;
         _view.user_input_lost ();
         
         ret_val = FALSE;
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
Name : trace
==============================================================================
*/

void  ViewImpl::trace (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   switch (message)
   {
   // mouse moves
   case WM_MOUSEMOVE:
      ohm_util_TRACE_INFO ("WM_MOUSEMOVE");
      break;

   case WM_MOUSELEAVE:
      ohm_util_TRACE_INFO ("WM_MOUSELEAVE");
      break;
   
   // button up
   case WM_LBUTTONUP:
      ohm_util_TRACE_INFO ("WM_LBUTTONUP");
      break;

   case WM_MBUTTONUP:
      ohm_util_TRACE_INFO ("WM_MBUTTONUP");
      break;

   case WM_RBUTTONUP:
      ohm_util_TRACE_INFO ("WM_RBUTTONUP");
      break;
   
   // button down
   case WM_LBUTTONDOWN:
      ohm_util_TRACE_INFO ("WM_LBUTTONDOWN");
      break;

   case WM_MBUTTONDOWN:
      ohm_util_TRACE_INFO ("WM_MBUTTONDOWN");
      break;

   case WM_RBUTTONDOWN:
      ohm_util_TRACE_INFO ("WM_RBUTTONDOWN");
      break;
   
   // double click
   case WM_LBUTTONDBLCLK:
      ohm_util_TRACE_INFO ("WM_LBUTTONDBLCLK");
      break;

   case WM_MBUTTONDBLCLK:
      ohm_util_TRACE_INFO ("WM_MBUTTONDBLCLK");
      break;

   case WM_RBUTTONDBLCLK:
      ohm_util_TRACE_INFO ("WM_RBUTTONDBLCLK");
      break;
   
   // wheel
   case WM_MOUSEWHEEL:
      ohm_util_TRACE_INFO ("WM_MOUSEWHEEL");
      break;
   
   // key
   case WM_KEYDOWN:
      ohm_util_TRACE_INFO ("WM_KEYDOWN");
      break;

   case WM_SYSKEYDOWN:
      ohm_util_TRACE_INFO ("WM_SYSKEYDOWN");
      break;

   case WM_KEYUP:
      ohm_util_TRACE_INFO ("WM_KEYUP");
      break;

   case WM_SYSKEYUP:
      ohm_util_TRACE_INFO ("WM_SYSKEYUP");
      break;

   // char     
   case WM_CHAR:
      ohm_util_TRACE_INFO ("WM_CHAR");
      break;

   case WM_UNICHAR:
      ohm_util_TRACE_INFO ("WM_UNICHAR");
      break;

   case  WM_CAPTURECHANGED:
      ohm_util_TRACE_INFO ("WM_UNICHAR");
      break;

   default:
      ohm_util_TRACE_INFO ("WM_???????");
      break;
   }
}



/*
==============================================================================
Name : handle_mouse_move
Description :
   The mouse moved over the window.
==============================================================================
*/

void  ViewImpl::handle_mouse_move (::LPARAM lparam)
{
   const float    x = float (GET_X_LPARAM (lparam));
   const float    y = float (GET_Y_LPARAM (lparam));

   const int      press_mask = MK_LBUTTON | MK_RBUTTON | MK_MBUTTON;
   
   if (! _mouse_in_flag)
   {
#if defined ohm_opal_VIEW_USE_EVENT_MGR
      _sys_event._mouse_location = opa::Point (x, y);
      _view.handle_system_event (_sys_event);
#else
      _view.mouse_moved (opa::Point (x, y), opa::Point (0, 0));
#endif
      ::TRACKMOUSEEVENT    event_track;
      event_track.cbSize = sizeof (event_track);
      event_track.dwFlags = TME_LEAVE;
      event_track.hwndTrack = _hwnd;
      event_track.dwHoverTime = HOVER_DEFAULT;
      
      bool ok_flag = TrackMouseEvent (&event_track);
      assert (ok_flag);
   }
   else
   {
#if defined ohm_opal_VIEW_USE_EVENT_MGR
      _sys_event._mouse_location = opa::Point (x, y);
      _view.handle_system_event (_sys_event);
#else
      const float    dx = float (math::round (x - _mouse_position._x));
      const float    dy = float (math::round (y - _mouse_position._y));

      _view.mouse_moved (opa::Point (x, y), opa::Point (dx, dy));
#endif
   }

   _mouse_position._x = x;
   _mouse_position._y = y;
   
   _mouse_in_flag = true;
}



/*
==============================================================================
Name : handle_mouse_leave
Description :
   The mouse moved over the window.
==============================================================================
*/

void  ViewImpl::handle_mouse_leave ()
{
#if defined ohm_opal_VIEW_USE_EVENT_MGR
   _sys_event._mouse_exited_view_flag = true; 
   _view.handle_system_event (_sys_event);
#else
   ::POINT        pt;
   ::GetCursorPos (&pt);
   
   ::RECT         window_rect;
   ::GetWindowRect (_hwnd, &window_rect);
   
   opa::Point     location (
      float (pt.x - window_rect.left),
      float (pt.y - window_rect.top)
   );
   const float    dx = float (math::round (location._x - _mouse_position._x));
   const float    dy = float (math::round (location._y - _mouse_position._y));
   _mouse_position = location;
   
   _view.mouse_exited_view (location, opa::Point (dx, dy));
#endif
   _mouse_in_flag = false;
}



/*
==============================================================================
Name : handle_mouse_click
==============================================================================
*/

void  ViewImpl::handle_mouse_click (::UINT message, ::LPARAM lparam)
{
   acquire_keyboard_focus ();

   bool press_flag = true;
   
   switch (message)
   {
   case WM_LBUTTONUP:
   case WM_MBUTTONUP:
   case WM_RBUTTONUP:
      press_flag = false;
      break;
   }
   
   int button = 0;
   
   switch (message)
   {
   case WM_LBUTTONUP:
   case WM_LBUTTONDOWN:
   case WM_LBUTTONDBLCLK:
      button = MButBit_L;
      break;

   case WM_RBUTTONUP:
   case WM_RBUTTONDOWN:
   case WM_RBUTTONDBLCLK:
      button = MButBit_R;
      break;

   case WM_MBUTTONUP:
   case WM_MBUTTONDOWN:
   case WM_MBUTTONDBLCLK:
      button = MButBit_M;
      break;
   }
   
   assert (button >= 0);
   assert (button < MButBit_NBR_ELT);

   if (press_flag)
   {
      _mouse_state |= 1 << button;
   }
   else
   {
      _mouse_state &= ~(1 << button);
   }

   if (_mouse_state != 0 && ! _system_mouse_capture_flag)
   {
      _system_mouse_capture_flag = true;

      // Only the topmost window can set the capture. We first ensure
      // that our window is brought to front.
      const ::HWND   main_hwnd = _window.get_system_window ();
      const ::HWND   foreground_wnd = ::GetForegroundWindow ();
      if (foreground_wnd != main_hwnd)
      {
         const ::HWND   ret_win = ::SetActiveWindow (_hwnd);

         // Possible failure
         if (ret_win == 0)
         {
            ohm_util_TRACE_WARNING (
               "Mouse capture failed because the application window "
               "cannot be activated."
            );
            _system_mouse_capture_flag = false;
         }
      }

      if (_system_mouse_capture_flag)
      {
         ::SetCapture (_hwnd);
      }
   }

   else if (_mouse_state == 0 && _system_mouse_capture_flag)
   {
      // Set it first, because it needs to be false during
      // the WM_CAPTURECHANGED processing.
      _system_mouse_capture_flag = false;

      ::ReleaseCapture ();
   }
   
   // for now ignore third button (middle button)
   // (see EventDispatcherStandardMouse::Button)
   
   if (button != MButBit_M)
   {
#if defined ohm_opal_VIEW_USE_EVENT_MGR
      ohm_lang_CHECK_CST (MouseButL, MButBit_L == SystemEvent::MouseButton_L);
      ohm_lang_CHECK_CST (MouseButR, MButBit_R == SystemEvent::MouseButton_R);

      if (press_flag)
      {
         const float    x = float (GET_X_LPARAM (lparam));
         const float    y = float (GET_Y_LPARAM (lparam));

         opa::Point mouse_click_position (x, y);
         opa::Point click_pos_delta = mouse_click_position - _mouse_click_position;

         bool dbl_flag = true;
         
         double now_s = sys::TimerReliable::use_instance ().get ();
         int & click_cnt = _click_cnt_arr [button];
         double & click_time_s = _click_time_s_arr [button];
         
         dbl_flag &= now_s < click_time_s + _click_duration_s;
         dbl_flag &= opa::Size (click_pos_delta._x, click_pos_delta._y).norm_1 () < 3;
         
         if (dbl_flag)
         {
            click_cnt += 1;
         }
         else
         {
            click_cnt = 1;
         }
         
         click_time_s = now_s;
         _mouse_click_position = mouse_click_position;
         
         _sys_event._click_cnt_arr [button] = click_cnt; 
         _sys_event._mouse_location = opa::Point (x, y); 
      }
      else
      {
         _sys_event._click_cnt_arr [button] = 0; 
      }

      _view.handle_system_event (_sys_event);
#else
      // button nbr start from 1 (see EventDispatcherStandardMouse::Button)
      
      int opal_button = button + 1;
      
      if (press_flag)
      {
         const float    x = float (GET_X_LPARAM (lparam));
         const float    y = float (GET_Y_LPARAM (lparam));
         
         opa::Point mouse_click_position (x, y);
         opa::Point click_pos_delta = mouse_click_position - _mouse_click_position;
         
         
         bool dbl_flag = true;
         
         double now_s = sys::TimerReliable::use_instance ().get ();
         int & click_cnt = _click_cnt_arr [button];
         double & click_time_s = _click_time_s_arr [button];
         
         dbl_flag &= now_s < click_time_s + _click_duration_s;
         dbl_flag &= opa::Size (click_pos_delta._x, click_pos_delta._y).norm_1 () < 3;
         
         if (dbl_flag)
         {
            click_cnt += 1;
         }
         else
         {
            click_cnt = 1;
         }
         
         click_time_s = now_s;
         _mouse_click_position = mouse_click_position;
         
         _view.mouse_moved (opa::Point (x, y), opa::Point::_zero);
         _view.mouse_button_pressed (opal_button, click_cnt);
      }
      else
      {
         _view.mouse_button_released (opal_button);
      }
#endif
   }
}



/*
==============================================================================
Name : handle_mouse_wheel
==============================================================================
*/

void  ViewImpl::handle_mouse_wheel (::WPARAM wparam)
{
   const bool  shift_flag = short (LOWORD(wparam)) & MK_SHIFT;
   const float delta_z = float (GET_WHEEL_DELTA_WPARAM (wparam)) / float (WHEEL_DELTA);

   opa::Point delta;
   
   if (shift_flag)
   {
      delta = opa::Point (delta_z, 0);
   }
   else
   {
      delta = opa::Point (0, delta_z);
   }

#if defined ohm_opal_VIEW_USE_EVENT_MGR
   _sys_event._wheel_delta = delta;
   
   _view.handle_system_event (_sys_event);
#else
   _view.mouse_wheel_moved (delta);
#endif
}



/*
==============================================================================
Name : handle_keyboard
Documentation :
   http://msdn.microsoft.com/en-us/library/ms646280(v=vs.85).aspx
==============================================================================
*/

LRESULT  ViewImpl::handle_keyboard (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ::LRESULT      ret_val = TRUE;

   ohm::opal::Key key (wparam, lparam);
   
   const bool repeated_flag = ((lparam >> 30) & 1) == 1;
   
   const bool modifier_flag = 
      (wparam == VK_CONTROL) ||
      (wparam == VK_RCONTROL) ||
      (wparam == VK_LCONTROL) ||
      (wparam == VK_SHIFT) ||
      (wparam == VK_LSHIFT) ||
      (wparam == VK_RSHIFT) ||
      (wparam == VK_MENU) ||
      (wparam == VK_LMENU) ||
      (wparam == VK_LMENU)
   ;

   switch (message)
   {
   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
#if defined ohm_opal_VIEW_USE_EVENT_MGR
      if (modifier_flag && repeated_flag)
      {
         // ignore repetition for modifiers
         _key_catched_flag = true;
      }
      else
      {
         _key_catched_flag = key_pressed (key, repeated_flag);
      }
#else
      if (repeated_flag)
      {
         if (modifier_flag)
         {
            // ignore repetition for modifiers
            _key_catched_flag = true;
         }
         else
         {
            _key_catched_flag = _view.key_repeated (key);
         }
      }
      else
      {
         _key_catched_flag = _view.key_pressed (key);
      }
#endif
      break;

   case WM_KEYUP:
   case WM_SYSKEYUP:
#if defined ohm_opal_VIEW_USE_EVENT_MGR
      _sys_event._key = key;
      _sys_event._key_change = SystemEvent::KeyChange_RELEASED;
      _key_catched_flag = _view.handle_system_event (_sys_event);
#else
      _key_catched_flag = _view.key_released (key);
#endif
      break;
   }
   
   if (_key_catched_flag)
   {
      ret_val = FALSE;
   }
   else if (!deferred_flag)
   {
      ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
   }

   return ret_val;
}



/*
==============================================================================
Name : handle_character
==============================================================================
*/

LRESULT  ViewImpl::handle_character (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ::LRESULT      ret_val = TRUE;

   if (!_key_catched_flag)
   {
      ohm::txt::unicode::Char ucs4 = wparam;
      
      std::string utf8;
      int err = ohm::txt::utf8::Codec8::encode_char (utf8, ucs4);
      
      if (err == 0)
      {
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _key_catched_flag = characters_typed (utf8);
#else
         _key_catched_flag = _view.characters_typed (utf8);
#endif         
         if (_key_catched_flag)
         {
            ret_val = FALSE;
         }
      }
   }
   
   if (_key_catched_flag)
   {
      ret_val = FALSE;
   }
   if (!deferred_flag)
   {
      ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
   }
   
   _key_catched_flag = false;

   return ret_val;
}



/*
==============================================================================
Name : fill_drag_data
==============================================================================
*/

void  ViewImpl::fill_drag_data (Data & data, HDROP hdrop)
{
   std::vector <::WCHAR>   filename;
   ::WCHAR        url_0 [INTERNET_MAX_URL_LENGTH + 1];

   const ::UINT   file_count = ::DragQueryFile (hdrop, -1, 0, 0);
   assert (file_count > 0);

   for (::UINT i = 0; i < file_count; ++i)
   {
      ::UINT         res = ::DragQueryFileW (hdrop, i, 0, 0);
      if (res < 0)
      {
         ohm_util_TRACE_ERROR2 (
            "fill_drag_data(): DragQueryFileW returned %1% while querying "
            "for the #%2% filename size.",
            res,
            i
         );
      }
      else
      {
         const long     fname_size = res;
         filename.resize (fname_size + 1);   // + Null terminator

         res = ::DragQueryFileW (hdrop, i, &filename [0], fname_size + 1);
         if (res == 0)
         {
            assert (false);
            ohm_util_TRACE_ERROR2 (
               "fill_drag_data(): DragQueryFileW returned %1% while querying "
               "for the #%2% filename data.",
               res,
               i
            );
         }
         else
         {
            // Potential issue: the path part of the URL is limited to 2048
            // characters. With Unicode + escaping, even small pathes could
            // generate overflows.
            ::DWORD        buf_len = INTERNET_MAX_URL_LENGTH;
            ::HRESULT      hresult =
               ::UrlCreateFromPathW (&filename [0], url_0, &buf_len, 0);

            if ((hresult != S_OK) && (hresult != S_FALSE))
            {
               assert (false);
               ohm_util_TRACE_ERROR1 (
                  "fill_drag_data(): UrlCreateFromPathW failed, returned %1%.",
                  hresult
               );
            }
            else
            {
               std::string    url_utf8;
               const int      ret_val =
                  ohm::txt::conv_utf16_to_utf8 (url_utf8, url_0);
               if (ret_val == txt::Err_OK)
               {
                  data._mime_type_data_map_arr.push_back (
                     Data::MimeTypeDataMap ()
                  );
                  Data::MimeTypeDataMap & map_new =
                     data._mime_type_data_map_arr.back ();

                  std::vector <archi::UByte> & raw_data
                     = map_new [Data::_uti_file_url_0];

                  raw_data.assign (url_utf8.begin (), url_utf8.end ());
                  raw_data.push_back (0);
               }
            }
         }
      }
   }
}



/*
==============================================================================
Name : process_event
==============================================================================
*/

void  ViewImpl::process_event (task::Event & event)
{
   EventType event_type = event.pop <EventType> ();

   switch (event_type)
   {
   case EventType_DRAG:
      {
         const opa::Point location = event.pop <opa::Point> ();
         mem::SharedPtr <Data> data_sptr = event.pop <mem::SharedPtr <Data> > ();

         assert (data_sptr.get () != 0);

         if (data_sptr.get () != 0)
         {
            _view.mouse_data_drag_set (*data_sptr);
            
#if defined ohm_opal_VIEW_USE_EVENT_MGR
            _sys_event._mouse_location = location;
            _sys_event._has_drag_data_flag = true;
            _view.handle_system_event (_sys_event);
#else
            // quick fix - drag data must be set earlier, and no mouse move here
            _view.mouse_moved (
               location,
               opa::Point (0, 0)
            );
#endif
         }
      }
      break;

   case EventType_DROP:
      {
         const opa::Point location = event.pop <opa::Point> ();
         mem::SharedPtr <Data> data_sptr = event.pop <mem::SharedPtr <Data> > ();

         _view.mouse_data_drag_set (*data_sptr);
         
#if defined ohm_opal_VIEW_USE_EVENT_MGR
         _sys_event._key._shift_flag   = ((::GetKeyState (VK_SHIFT  ) & 0x8000) != 0);
         _sys_event._key._command_flag = ((::GetKeyState (VK_CONTROL) & 0x8000) != 0);
         _sys_event._key._alt_flag     = ((::GetKeyState (VK_MENU   ) & 0x8000) != 0);
         _sys_event._mouse_location = location;
         _sys_event._drag_data_dropped_flag = true;
         _view.handle_system_event (_sys_event);
#else
         _view.mouse_data_dropped (
            location,
            opa::Point (0, 0)
         );
#endif
      }
      break;

   case EventType_DEFER_MSG:
      {
         ::HWND hwnd = event.pop <::HWND> ();
         ::UINT msg = event.pop <::UINT> ();
         ::WPARAM wparam = event.pop <::WPARAM> ();
         ::LPARAM lparam = event.pop <::LPARAM> ();

         handle_event_actual (true, hwnd, msg, wparam, lparam);
      }
      break;
   }
}



const txt::Char16 ViewImpl::_class_name_0 [] = L"Ohm_Force_Opal_ViewImpl_Class_Name";
long  ViewImpl::_window_cnt = 0;
bool  ViewImpl::_registered_flag = false;
const double   ViewImpl::_click_duration_s = ::GetDoubleClickTime () / 1000.0;
std::map <::HWND, ViewImpl *> ViewImpl::_view_map;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
