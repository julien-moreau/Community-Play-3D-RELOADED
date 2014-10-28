/*****************************************************************************

        ViewImpl.h
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



#if ! defined (ohm_opal_ViewImpl_HEADER_INCLUDED)
#define  ohm_opal_ViewImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX

#include "ohm/opal/def.h"

#if (ohm_opal_API != ohm_opal_API_DIRECTX)
   #error Windows version of ViewImpl works only with DirectX API
#endif

#include "ohm/opa/Rect.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/directx/ViewImplDirectX.h"
#include "ohm/opal/private/SystemEvent.h"
#include "ohm/sys/win32/LocalUseOle.h"
#include "ohm/task/Observer.h"
#include "ohm/txt/Char16.h"

// include <windows.h> before <shellapi.h>
#include <windows.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <winbase.h>
#include <winuser.h>
#include <OleIdl.h>



namespace ohm
{
namespace opal
{



class View;
class Window;
class WindowImpl;
class Data;

class ViewImpl
: public ::IDropTarget
{

   friend class WindowImpl;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ViewImpl (Window & window, View & view);
   virtual        ~ViewImpl ();
   
   // internal
   void           vt_init ();
   void           vt_restore ();
   void           vt_update (const opa::Size & size);
   void           vt_draw (bool wait_vbr_flag);

   // internal 
   bool           key_pressed (const opal::Key & key, bool repeated_flag);
   bool           characters_typed (const std::string & typed_utf8);
   
   ::HWND         get_system_view () const;
   View &         use_view ();
   
   void           set_origin (const opa::Point & point);
   void           set_bounds (const opa::Size & size);
   opa::Size      get_bounds ();
   void           post_view_layout_changed ();

   void           set_nbr_skip_frame (int nbr);
   
   void           show ();
   void           hide ();
   
   void           invalidate (const opa::Rect & rect);
   void           invalidate_all ();
   
   void           acquire_keyboard_focus ();
   void           release_keyboard_focus ();

   void           notify_capture_changed ();

   void           debug_toggle_show_invalid_rect ();  

   static ViewImpl *
                  get (::HWND hwnd);

   // IUnknown interface
   virtual ::HRESULT __stdcall
                  QueryInterface (REFIID riid, void ** interface_ptr_ptr);
   virtual ::ULONG __stdcall
                  AddRef ();
   virtual ::ULONG __stdcall
                  Release ();
   // IDropTarget interface
   virtual ::HRESULT __stdcall 
                  DragEnter (::IDataObject * data_obj_ptr, ::DWORD key_state, ::POINTL location, ::DWORD * drop_effect_ptr);
   virtual ::HRESULT __stdcall
                  DragOver (::DWORD key_state, ::POINTL location, ::DWORD * drop_effect_ptr);
   virtual ::HRESULT __stdcall
                  DragLeave ();
   virtual ::HRESULT __stdcall
                  Drop (::IDataObject * data_obj_ptr, ::DWORD key_state, ::POINTL location, ::DWORD *drop_effect_ptr);
       
 

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private: 

   // Bit indexes for the mouse buttons.
   enum MButBit
   {
      MButBit_L = 0, // Left
      MButBit_R,     // Right
      MButBit_M,     // Middle

      MButBit_NBR_ELT
   };
   
   enum EventType
   {
      EventType_DRAG = 0,
      EventType_DROP,
      EventType_DEFER_MSG
   };

   static LRESULT CALLBACK 
                  handle_event_proc (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   LRESULT        handle_event (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   LRESULT        handle_event_auto (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   LRESULT        handle_event_actual (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   
   static void    trace (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   
   void           handle_mouse_move (::LPARAM lparam);
   void           handle_mouse_leave ();
   void           handle_mouse_click (::UINT message, ::LPARAM lparam);
   void           handle_mouse_wheel (::WPARAM wparam);

   LRESULT        handle_keyboard (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   LRESULT        handle_character (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);

   void           fill_drag_data (Data & data, HDROP hdrop);
   
   // task::Observer notifications
   void           process_event (task::Event & event);
   

   Window &       _window;
   View &         _view;
   
   ::HWND         _hwnd;

   opa::Point     _mouse_position;
   opa::Point     _mouse_click_position;
   bool           _mouse_in_flag;
   bool           _relative_hidden_flag;
   bool           _block_flag;

   ViewImplDirectX
                  _impl;
   
   bool           _key_catched_flag;

   int            _mouse_state;     // Flags from MButBit.
   bool           _system_mouse_capture_flag;
   
   int            _click_cnt_arr [MButBit_NBR_ELT];
   double         _click_time_s_arr [MButBit_NBR_ELT];

   task::Observer _observer;

   // OLE stuff
   sys::win32::LocalUseOle
                  _ole_session;
   long           _ole_ref_count;
   mem::SharedPtr <Data>
                  _drag_data_sptr;

   SystemEvent    _sys_event;

   static const txt::Char16
                  _class_name_0 [];
   static long    _window_cnt;
   static bool    _registered_flag;
   static const double
                  _click_duration_s;
   static std::map <::HWND, ViewImpl *>
                  _view_map;

   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ViewImpl (const ViewImpl &other);
   ViewImpl &     operator = (const ViewImpl &other);
   bool           operator == (const ViewImpl &other);
   bool           operator != (const ViewImpl &other);

}; // class ViewImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ViewImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
