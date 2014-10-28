/*****************************************************************************

        ApplicationImpl.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70971

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



#if ! defined (ohm_opal_ApplicationImpl_HEADER_INCLUDED)
#define  ohm_opal_ApplicationImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX

#include "ohm/opal/Application.h"
#include "ohm/txt/Char16.h"

#include <windows.h>

#include <list>
#include <string>
#include <vector>



namespace ohm
{
namespace opal
{



class Window;
class View;
class HIDEvent;

class ApplicationImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ApplicationImpl ();
   
   static ApplicationImpl &
                  use_instance ();
   
   void           quit ();
   
   void           set_ui_mode_windowed ();
   void           set_ui_mode_immersive ();
   
   void           set_system_pasteboard (const Data & data);
   bool           is_system_pasteboard_external_available ();
   void           get_system_pasteboard (Data & data);
   
   void           set_cursor (StandardMouseCursor cursor);
   void           set_default_cursor ();
   void           release_cursor (void * ptr);
   void           show_cursor ();
   void           hide_cursor ();
   void           obscure_cursor ();
   void           unobscure_cursor ();
   void           reposition_mouse (const opa::Point & global_location);
   
   void           choose_file (std::string & path, const std::string & original_path, bool (*filter_proc) (const char * /*path_0*/), const std::string & window_title, const std::string & message);
   void           choose_file (std::string & path, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message);
   void           choose_files (std::list <std::string> & path_list, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message);
   void           choose_folder (std::string & path, const std::string & original_path, const std::string & window_title, const std::string & message);
   void           choose_volume (std::string & path, const std::string & window_title, const std::string & message);
   void           put_file (std::string & path, const std::string & original_path, const std::string & original_filename, const std::string & window_title, const std::string & message, const std::vector <std::string> &filter);
   
   void           set_hinst (::HINSTANCE hinst);
   ::HINSTANCE    get_hinst ();  

   View *         get_keyboard_focus_ptr ();
   void           synthetize_events (View & view);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ApplicationImpl ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   static ::LRESULT CALLBACK 
                  handle_event_proc (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   static void    trace (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam);
   
   static void    handle_crumb_proc (const ::MSG & msg, void * user_data);
   void           handle_crumb (const ::MSG & msg);
   void           handle_crumb_keyboard (HIDEvent & event, const ::MSG & msg);
   void           handle_crumb_character (HIDEvent & event, const ::MSG & msg);
   
   ::HINSTANCE    _hinst;
   bool           _cursor_hidden_flag;

   ::HWND         _clipboard_hwnd;

   static const txt::Char16
                  _clipboard_class_name_0 [];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ApplicationImpl (const ApplicationImpl &other);
   ApplicationImpl &
                  operator = (const ApplicationImpl &other);
   bool           operator == (const ApplicationImpl &other);
   bool           operator != (const ApplicationImpl &other);

}; // class ApplicationImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ApplicationImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
