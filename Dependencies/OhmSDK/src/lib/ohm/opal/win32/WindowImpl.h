/*****************************************************************************

        WindowImpl.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66159

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



#if ! defined (ohm_opal_WindowImpl_HEADER_INCLUDED)
#define  ohm_opal_WindowImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"
#include "ohm/task/Observer.h"
#include "ohm/txt/String16.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#include <windows.h>

#include <string>
#include <set>



namespace ohm
{
namespace opal
{



class Window;
class View;
class ViewImpl;

class WindowImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  WindowImpl (Window & window);
   virtual        ~WindowImpl ();
   
   void           set_class_document ();
   void           set_class_owned (archi::Int64 owner_window_data);
   void           set_class_floating ();
   void           set_class_no_decoration ();
   void           set_class_floating_no_decoration ();
   void           set_class_floating_overlay_group ();
   void           set_class_dialog ();
   void           set_class_overlay ();
   void           set_overlay_group ();
   void           set_no_shadow ();
   void           set_properties (bool close_box_flag, bool zoom_box_flag, bool collapse_box_flag, bool resizable_flag);

   bool           is_class_document () const;
   
   void           create ();
   
   void           show ();
   void           hide ();
   void           collapse ();
   
   void           set_origin (const opa::Point & origin);
   opa::Point     get_origin ();
   opa::Point     get_content_origin ();
   void           reposition_center (Window * parent_window_ptr, bool screen_relative_flag);
   void           reposition_alert (Window * parent_window_ptr, bool screen_relative_flag);
   void           resize_default_main_screen ();
   
   void           set_bounds (const opa::Size & bounds);
   opa::Size      get_bounds ();
   void           set_limit_bounds (const opa::Size & min_bounds, const opa::Size & max_bounds);
   
   bool           is_active ();
   
   void           set_title (const std::string & title);
   void           set_alpha (float alpha);
   void           set_modified (bool modified_flag);
   void           set_proxy_path (const std::string & proxy_path);
   
   void           window_bounds_changed ();

   ::HWND         get_system_window () const;
   Window &       get_window ();
   
   void           bind_menu_bar ();


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class ViewImpl;

   typedef  std::set <View *> ViewSet;

   static ::LRESULT CALLBACK 
                  handle_event_proc (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   ::LRESULT      handle_event (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   ::LRESULT      handle_event_actual (bool deferred_flag, ::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   
   void           add_view (View * view_ptr);
   void           remove_view (View * view_ptr);
   void           update_bounds ();
   void           update_title ();

   // task::Observer notifications
   void           process_event (task::Event & event);

   static const txt::Char16
                  _class_name_0 [];

   Window &       _window;
   
   opa::Size      _min_bounds;
   opa::Size      _max_bounds;
   opa::Size      _cur_bounds;
   ViewSet        _view_ptr_arr;
   
   ::DWORD        _style;
   ::DWORD        _style_ex;
   ::HWND         _hwnd;
   ::HWND         _hwnd_owner;
   bool           _menu_owner_flag;
   bool           _modified_flag;
   txt::String16  _title;     // UTF-16

   task::Observer _observer;
   View *         _last_focus_ptr;

   static long    _window_cnt;
   static bool    _registered_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WindowImpl (const WindowImpl &other);
   WindowImpl &      operator = (const WindowImpl &other);
   bool           operator == (const WindowImpl &other);
   bool           operator != (const WindowImpl &other);

}; // class WindowImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_WindowImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
