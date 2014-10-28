/*****************************************************************************

        ApplicationImpl.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70201

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

#include "ohm/opal/StandardMouseCursor.h"
#include "ohm/opal/Application.h"
#include "ohm/opa/Point.h"

#include <list>
#include <string>
#include <vector>



namespace ohm
{
namespace opal
{



class Window;
class View;

class ApplicationImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ApplicationImpl ();
   
   static ApplicationImpl &
                  use_instance ();
   
   void           set_ui_mode_windowed ();
   void           set_ui_mode_immersive ();
   
   void           set_system_pasteboard (const Data & data);
   bool           is_system_pasteboard_external_available ();
   void           get_system_pasteboard (Data & data);
   
   void           set_cursor (StandardMouseCursor cursor);
   void           set_default_cursor ();
   void           show_cursor ();
   void           hide_cursor ();
   void           obscure_cursor ();
   void           unobscure_cursor ();
   
   void           choose_file (std::string & path, const std::string & original_path, bool (*filter_proc) (const char * /*path_0*/), const std::string & window_title, const std::string & message);
   void           choose_file (std::string & path, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message);
   void           choose_files (std::list <std::string> & path_list, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message);
   void           choose_folder (std::string & path, const std::string & original_path, const std::string & window_title, const std::string & message);
   void           choose_volume (std::string & path, const std::string & window_title, const std::string & message);
   void           put_file (std::string & path, const std::string & original_path, const std::string & original_filename, const std::string & window_title, const std::string & message, const std::vector <std::string> &filter);
   
   void           begin_mouse_capture (HIViewRef view_ref, bool relative_hidden_flag, bool block_flag);
   void           end_mouse_capture (HIViewRef view_ref);
   void           end_mouse_capture (HIViewRef view_ref, const opa::Point & view_rel_pt);
   void           begin_mouse_gesture (HIViewRef view_ref, bool block_flag);
   void           end_mouse_gesture (HIViewRef view_ref);
   
   void           post_synthesized_mouse_move_event ();
   void           post_synthesized_key_event (::CGKeyCode key_code, bool key_down_flag);
   
   View *         get_keyboard_focus_ptr ();
   void           synthetize_events (View & view);

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ApplicationImpl ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef bool (*FilterProc) (const char * /*path_0*/);
   
   void           notify_activated ();
   void           notify_deactivated ();
   
   static ::Boolean
                  filter_file_proc (::AEDesc * item_ptr, void * info, ::NavCallBackUserData user_data, ::NavFilterModes filter_mode);
   ::Boolean      filter_file (::AEDesc * item_ptr, void * info, ::NavCallBackUserData user_data, ::NavFilterModes filter_mode);
   
   static OSStatus
                  handle_event_proc (EventHandlerCallRef handler_call_ref, EventRef event_ref, void * user_data);
   OSStatus       handle_event (EventHandlerCallRef handler_call_ref, EventRef event_ref);
   OSStatus       handle_application_event (EventRef event_ref);
   OSStatus       handle_mouse_event (EventRef event_ref);
   OSStatus       handle_window_event (EventRef event_ref);
   OSStatus       handle_keyboard_event (EventRef event_ref);
   OSStatus       handle_text_input_event (EventRef event_ref);
   OSStatus       handle_command_event (EventRef event_ref);
   
   Application::FileFormatList
                  _file_format_list;
   FilterProc     _filter_proc;
   
   PasteboardRef  _local_paste_board_ref;
   PasteboardRef  _global_paste_board_ref;
   
   EventHandlerRef
                  _app_event_handler_ref;
   
   HIViewRef      _capture_gesture_ref;
   bool           _relative_hidden_flag;
   bool           _capture_flag;
   bool           _block_flag;
   
   std::vector <HIDEvent>
                  _hid_event_keyboard_arr;



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
