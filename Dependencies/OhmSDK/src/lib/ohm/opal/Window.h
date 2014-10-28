/*****************************************************************************

        Window.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66185

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



#if ! defined (ohm_opal_Window_HEADER_INCLUDED)
#define  ohm_opal_Window_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/WindowImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/carbon/WindowImpl.h"

#else
   #error Operation System not supported

#endif

#include "ohm/opa/Size.h"
#include "ohm/opa/Point.h"
#include "ohm/util/Delegate.h"

#include <string>



namespace ohm
{
namespace opal
{

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   typedef ::HWND SystemWindow;

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   typedef WindowRef SystemWindow;

#else
   #error Operation System not supported

#endif




class Window
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      enum Type
      {
                  Type_CLOSING = 0,
                  Type_CLOSED,
      };
      
      Type        _type;
      bool        _close_flag;
   };
   
   static Window *
                  create_document_window (const std::string & name);

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   static Window *
                  create_owned_window (const std::string & name, bool close_box_flag, bool resizable_flag, archi::Int64 owner_window_data);
#endif

   static Window *
                  create_floating_window (const std::string & name, bool close_box_flag, bool resizable_flag);
   static Window *
                  create_floating_window_overlay_group (const std::string & name, bool close_box_flag, bool resizable_flag);
   static Window *
                  create_floating_no_decoration_window (const std::string & name);
   static Window *
                  create_no_decoration_window (const std::string & name);
   static Window *
                  create_no_decoration_window_overlay_group (const std::string & name);
   static Window *
                  create_no_decoration_no_shadow_window_overlay_group (const std::string & name);
   static Window *
                  create_overlay_window (const std::string & name);
   static Window *
                  create_dialog (Window & parent_window, const std::string & name);
   
   virtual        ~Window ();
   
   template <class Receiver, void (Receiver::*handle_event) (Event &)>
   void           bind (Receiver & receiver);

   void           bind_menu_bar ();

   void           show ();
   void           hide ();
   void           collapse ();
   
   void           run_dialog (); // for modality
   
   void           set_origin (const opa::Point & origin);
   opa::Point     get_origin ();
   opa::Point     get_content_origin ();
   void           set_bounds (const opa::Size & bounds);
   opa::Size      get_bounds ();
   void           set_limit_bounds (const opa::Size & min_bounds, const opa::Size & max_bounds);
   void           resize_default_main_screen ();
   
   bool           is_document () const;
   void           set_document ();
   void           set_no_decoration ();

   bool           is_active ();
   
   void           set_title (const std::string & title);

   void           set_alpha (float alpha);
      
   void           set_modified (bool modified_flag);
   void           set_proxy_path (const std::string & proxy_path);
   
   void           window_bounds_changed (); 
   
   SystemWindow   get_system_window () const;
   WindowImpl &   use_window_impl ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  Window ();

   static void    check_name (const std::string & name);
   
   // WindowImpl access
   bool           req_close ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class   WindowImpl;
   
   WindowImpl     _window_impl;
   
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Window (const Window &other);
   Window &       operator = (const Window &other);
   bool           operator == (const Window &other);
   bool           operator != (const Window &other);

}; // class Window



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Window.hpp"

#endif   // ohm_opal_Window_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
