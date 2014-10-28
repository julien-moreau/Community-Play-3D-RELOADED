/*****************************************************************************

        Application.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71263

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

#include "ohm/opal/Application.h"
#include "ohm/opal/Cursor.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/QuitListener.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/utf8/utf8.h"
#include "ohm/txt/Err.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/ApplicationImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/carbon/ApplicationImpl.h"

#else
   #error Operation System not supported

#endif

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Application::~Application ()
{
}



/*
==============================================================================
Name : create
Description :
   force instantiation of singleton
==============================================================================
*/

void  Application::create ()
{
   use_instance ();
   ApplicationImpl::use_instance ();
}



/*
==============================================================================
Name : use_intance
==============================================================================
*/

Application &  Application::use_instance ()
{
   static Application instance;
   
   return instance;
}



/*
==============================================================================
Name : run
Description :
   Run the event loop for that application.
==============================================================================
*/

void  Application::run ()
{
   task::RunLoopGui::use_instance ().run ();
}
   

            
/*
==============================================================================
Name : request_quit
==============================================================================
*/

bool  Application::request_quit ()
{
   //bool can_quit_flag = task::RunLoopGui::use_instance ().can_quit ();

   // to do : we need an external system for this, not relying
   // on task
   bool can_quit_flag = true;
   
   if (can_quit_flag)
   {
      task::RunLoopGui::use_instance ().stop ();
   }
   
   return can_quit_flag;
}



/*
==============================================================================
Name : set_ui_mode_windowed
Description :
   Change user interface mode to windowed mode. This is the default mode
   in which should be the application.
   In that mode, the desktop decoration (like task bar on Windows, dock on
   MacOS X) should be present (activated)
==============================================================================
*/

void  Application::set_ui_mode_windowed ()
{
   ApplicationImpl::use_instance ().set_ui_mode_windowed ();
}



/*
==============================================================================
Name : set_ui_mode_immersive
Description :
   Change user interface mode to immersive mode.
   In that mode, the desktop decoration (like task bar on Windows, dock on
   MacOS X) are deactivated :
   - Task bar on Windows is not visible, and will not show up if the
   mouse cursor goes onto the taskbar activate aera
   - The dock is prevented to show on MacOS X
   - Menubar on mac is invisible and will not show itself even when going
     to the menubar location.
==============================================================================
*/

void  Application::set_ui_mode_immersive ()
{
   ApplicationImpl::use_instance ().set_ui_mode_immersive ();
}



/*
==============================================================================
Name : set_pasteboard
Description :
   Change the actual content of the pasteboard.
   This may come from a request through the opal system, or maybe
   actualised when the application is switched, or if there is a mechanism
   on the os system messages to notify of a system pastebord change.
==============================================================================
*/

void  Application::set_pasteboard (const Data & data)
{
   ApplicationImpl::use_instance ().set_system_pasteboard (data);
   
   _pasteboard = data;
}



/*
==============================================================================
Name : get_pasteboard
==============================================================================
*/

void  Application::get_pasteboard (Data & data)
{
   if (ApplicationImpl::use_instance ().is_system_pasteboard_external_available ())
   {
      ApplicationImpl::use_instance ().get_system_pasteboard (data);
   }
   else
   {
      data = _pasteboard;
   }
}



/*
==============================================================================
Name : process_command
Description :
   Send the command to the application, then the window having focus, then if
   the command is not catched to the first active non floating window (if it
   is not the same window)
==============================================================================
*/

void  Application::process_command (const std::string & command)
{
   /*WindowRef user_focus_window_ref = GetUserFocusWindow ();
   WindowRef front_window_ref = ActiveNonFloatingWindow ();
   
   MsgHandlerComponent::MsgHandlerReply reply = MsgHandlerComponent::MsgHandlerReply_PASS;
   
   if (reply == MsgHandlerComponent::MsgHandlerReply_PASS)
   {
      reply = do_process_command (command);
   }
   
   if (reply == MsgHandlerComponent::MsgHandlerReply_PASS)
   {
      if (user_focus_window_ref != 0)
      {
         Window * window_ptr = get_window (user_focus_window_ref);
      
         if (window_ptr != 0)
         {
            reply = window_ptr->process_command (command);
         }
      }
   }
   
   if (reply == MsgHandlerComponent::MsgHandlerReply_PASS)
   {
      if ((front_window_ref != 0) && (user_focus_window_ref != front_window_ref))
      {
         Window * window_ptr = get_window (front_window_ref);
      
         if (window_ptr != 0)
         {
            reply = window_ptr->process_command (command);
         }
      }
   }
   
   if (reply == MsgHandlerComponent::MsgHandlerReply_PASS)
   {
      WindowRef window_ref = GetWindowList ();
      
      while (window_ref != 0)
      {
         if ((window_ref != front_window_ref) && (window_ref != user_focus_window_ref))
         {
            WindowClass window_class;
            OSStatus err = GetWindowClass (window_ref, &window_class);
            assert (err == 0);
            
            if (window_class == kFloatingWindowClass)
            {
               Window * window_ptr = get_window (window_ref);
            
               if (window_ptr != 0)
               {
                  reply = window_ptr->process_command (command);
               }
            }
         }
         
         window_ref = GetNextWindow (window_ref);
      }
   }*/
}



/*
==============================================================================
Name : set
Application :
   Set the mouse cursor to 'cursor'
Note :
   This makes an optimization by avoiding to set 'cursor' when not necessary
   (ie. already set) and to avoid flickering.
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set (opal::Cursor & cursor)
{
#if ! defined (ohm_opal_APPLICATION_FORCE_CURSOR_FLAG)
   if (_cursor_ptr != &cursor)
#endif
   {
      cursor.set ();

#if ! defined (ohm_opal_APPLICATION_FORCE_CURSOR_FLAG)      
      _cursor_ptr = &cursor;
#endif
   }
}
#endif



/*
==============================================================================
Name : set
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set (StdCursor cursor)
{
   switch (cursor)
   {
   case StdCursor_ARROW:
      set_cursor_arrow ();
      break;

   case StdCursor_ARROW_POPUP:
      set_cursor_arrow_popup ();
      break;

   case StdCursor_HAND_POINTING:
      set_cursor_hand_pointing ();
      break;

   case StdCursor_IBEAM:
      set_cursor_ibeam ();
      break;

   case StdCursor_RESIZE_VERTICAL:
      set_cursor_resize_vertical ();
      break;

   default:
      assert (false);
      set_cursor_arrow ();
      break;
   }
}
#endif



/*
==============================================================================
Name : set_cursor_arrow
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_arrow ()
{
   if (_cursor_theme_mac_flag)
   {
      set_cursor ("ohm.cursor.mac.arrow.png", opa::Point (3, 4));
   }
   else
   {
      set_cursor ("ohm.cursor.win.arrow.png", opa::Point (2, 2));
   }
}
#endif



/*
==============================================================================
Name : set_cursor_arrow_popup
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_arrow_popup ()
{
   if (_cursor_theme_mac_flag)
   {
      set_cursor ("ohm.cursor.mac.arrow.popup.png", opa::Point (3, 4));
   }
   else
   {
      set_cursor ("ohm.cursor.win.arrow.popup.png", opa::Point (2, 2));
   }
}
#endif



/*
==============================================================================
Name : set_cursor_hand_pointing
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_hand_pointing ()
{
   if (_cursor_theme_mac_flag)
   {
      set_cursor ("ohm.cursor.mac.hand.pointing.png", opa::Point (7, 2));
   }
   else
   {
      set_cursor ("ohm.cursor.win.hand.pointing.png", opa::Point (7, 2));
   }
}
#endif



/*
==============================================================================
Name : set_cursor_ibeam
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_ibeam ()
{
   if (_cursor_theme_mac_flag)
   {
      set_cursor ("ohm.cursor.mac.ibeam.png", opa::Point (3, 8));
   }
   else
   {
      set_cursor ("ohm.cursor.win.ibeam.png", opa::Point (3, 8));
   }
}
#endif



/*
==============================================================================
Name : set_cursor_resize_vertical
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_resize_vertical ()
{
   if (_cursor_theme_mac_flag)
   {
      set_cursor ("ohm.cursor.mac.resize.vertical.png", opa::Point (8, 8));
   }
   else
   {
      set_cursor ("ohm.cursor.win.resize.vertical.png", opa::Point (8, 8));
   }
}
#endif



/*
==============================================================================
Name : set_cursor_theme_mac
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_theme_mac ()
{
   _cursor_theme_mac_flag = true;
}
#endif



/*
==============================================================================
Name : set_cursor_theme_win
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor_theme_win ()
{
   _cursor_theme_mac_flag = false;
}
#endif



/*
==============================================================================
Name : set_cursor
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor (const std::string & path, const opa::Point & hot_pt)
{
   Cursor & cursor = CursorManager::use_instance ().get (path, hot_pt);
   
   set (cursor);
}
#endif



/*
==============================================================================
Name : release_cursor
Note :
   Application will release the cursor when the mouse is not over an opal
   managed view or when an application switch occur. This is related to
   'set' above, that will only change the cursor when necessary. When the
   cursor is not under control of our code, tell it to the system to force
   an update when needed.
==============================================================================
*/

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
#if ! defined (ohm_opal_APPLICATION_FORCE_CURSOR_FLAG)
void  Application::release_cursor ()
{
   _cursor_ptr = 0;
}
#endif
#endif



/*
==============================================================================
Name : set_cursor
Description :
   Set the mouse cursor to 'cursor'.
   The application maitains a stack of cursors with their associated mhcs.
   Here, if the stack already contains mhc, it is destacked and put at the
   top with 'cursor'.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor (void * ptr, int depth, StandardMouseCursor cursor)
{
   assert (cursor != StandardMouseCursor_NONE);
   assert (cursor != StandardMouseCursor_CUSTOM);
   
   CursorBundle cursor_bundle = erase_cursor (ptr);
   
   cursor_bundle._cursor_type = cursor;
   cursor_bundle._custom_cursor_ptr = 0;
   cursor_bundle._depth = depth;
   
   _cursor_stack.push_back (cursor_bundle);
   _cursor_stack_invalid_flag = true;
}
#endif



/*
==============================================================================
Name : set_cursor
Description :
   Set the mouse cursor to 'cursor'.
   The application maitains a stack of cursors with their associated mhcs.
   Here, if the stack already contains mhc, it is destacked and put at the
   top with 'cursor'.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::set_cursor (void * ptr, int depth, opal::Cursor & cursor)
{
   CursorBundle cursor_bundle = erase_cursor (ptr);
   
   cursor_bundle._cursor_type = StandardMouseCursor_CUSTOM;
   cursor_bundle._custom_cursor_ptr = &cursor;
   cursor_bundle._depth = depth;
   
   _cursor_stack.push_back (cursor_bundle);
   _cursor_stack_invalid_flag = true;
}
#endif



/*
==============================================================================
Name : release_cursor
Description :
   Release the mouse cursor associated to 'mhc'
   The application maitains a stack of cursors with their associated mhcs.
   Here, it will destack the cursor associated to mhc in the stack. If mhc
   was on top of the stack, then the next cursor on the stack (the top of
   the stack) is set. If the stack is empty, then display default system
   cursor.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::release_cursor (void * ptr)
{
   erase_cursor (ptr);
   _cursor_stack_invalid_flag = true;
}
#endif



/*
==============================================================================
Name : show_cursor
==============================================================================
*/

void  Application::show_cursor (void * ptr, bool show_flag)
{
   CursorBundle * cursor_bundle_ptr = find_cursor (ptr);

   if (cursor_bundle_ptr != 0)
   {
      cursor_bundle_ptr->_visible_flag = show_flag;
      _cursor_stack_invalid_flag = true;
   }
}



/*
==============================================================================
Name : obscure_cursor
==============================================================================
*/

void  Application::obscure_cursor (void * ptr)
{
   CursorBundle * cursor_bundle_ptr = find_cursor (ptr);

   if (cursor_bundle_ptr != 0)
   {
      cursor_bundle_ptr->_obscured_flag = true;
      _cursor_stack_invalid_flag = true;
   }
}



/*
==============================================================================
Name : apply_cursor
Description :
   Used internally to apply cursor state.
   This is called by the system. It is aimed at reducing the number of
   cursor set in an event handling, to prevent visual artifact when cursor
   are changed quickly by the operating system.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Application::apply_cursor ()
{
   if (!_cursor_stack.empty ())
   {
      if (_cursor_stack_invalid_flag)
      {
         _cursor_stack.sort ();  // stable
         
         const CursorBundle & cursor_bundle = _cursor_stack.back ();
         StandardMouseCursor cursor_type = cursor_bundle._cursor_type;
         
         if (cursor_bundle._visible_flag && !_cursor_visible_flag)
         {
            ApplicationImpl::use_instance ().show_cursor ();
            
            _cursor_visible_flag = true;
         }
         else if ((!cursor_bundle._visible_flag) && _cursor_visible_flag)
         {
            ApplicationImpl::use_instance ().hide_cursor ();
            
            _cursor_visible_flag = false;
         }
         
         if (cursor_bundle._obscured_flag && !_cursor_obscured_flag)
         {
            ApplicationImpl::use_instance ().obscure_cursor ();
            
            _cursor_obscured_flag = true;
         }
         else if ((!cursor_bundle._obscured_flag) && _cursor_obscured_flag)
         {
            ApplicationImpl::use_instance ().unobscure_cursor ();
            
            _cursor_obscured_flag = false;
         }
         
         if (cursor_type == StandardMouseCursor_CUSTOM)
         {
            _cursor_type = StandardMouseCursor_CUSTOM;
            
            opal::Cursor * cursor_ptr = cursor_bundle._custom_cursor_ptr;
            assert (cursor_ptr != 0);
            
            if (_custom_cursor_ptr != cursor_ptr)
            {
               _custom_cursor_ptr = cursor_ptr;
               
               if (cursor_ptr != 0)
               {
                  cursor_ptr->set ();
               }
            }
         }
         else
         {
            _custom_cursor_ptr = 0;
            
            if (_cursor_type != cursor_type)
            {
               _cursor_type = cursor_type;
               
               assert (cursor_type != StandardMouseCursor_NONE);
               
               ApplicationImpl::use_instance ().set_cursor (cursor_type);
            }
         }
         
         _cursor_stack_invalid_flag = false;
      }
   }
   else
   {
      ApplicationImpl::use_instance ().show_cursor ();
      ApplicationImpl::use_instance ().set_default_cursor ();
   }
}
#endif



/*
==============================================================================
Name : choose_file
Description :
   Filter proc version.
   See 'choose_file' below.
==============================================================================
*/

void  Application::choose_file (std::string & path, const std::string & original_path, bool (*filter_proc) (const char * /*path_0*/), const std::string & window_title, const std::string & message)
{
   assert (&original_path != 0);
   assert (&window_title != 0);
   assert (&message != 0);
   assert (txt::utf8::Codec8::check_string_valid (original_path) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (window_title) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (message) == txt::Err_OK);
   
   View * view_ptr = ApplicationImpl::use_instance ().get_keyboard_focus_ptr ();
   
   ApplicationImpl::use_instance ().choose_file (
      path,
      original_path,
      filter_proc,
      window_title,
      message
   );

   // deblock after a possible modal loop eating task signals
   
   task::RunLoopGui::use_instance ().notify_post_external_modality ();
   
   // some events might need to be synthetized
   // for example the event might contain a modifier (like command) which
   // may have been released in this call
   
   if (view_ptr != 0)
   {
      ApplicationImpl::use_instance ().synthetize_events (*view_ptr);
   }

   assert (txt::utf8::Codec8::check_string_valid (path) == txt::Err_OK);
}



/*
==============================================================================
Name : choose_file
Description :
   Display a standard file navigation dialog where an individual file maybe
   chosen.
   'original_path' is the path where the navigation dialog should display
   when opened. If the string is empty, then this path is ignored, and
   the default system one is taken.
   'extension_list' is a list of string to match. No wild card, and the
   string match is done only to the end of the string
   'window_title' is the title to show in the dialog window decoration if
   any.
   'message' is the message to display around the file browser if possible.
   On return, 'path' is filled with the path of the chosen file, in fopen
   style paths, UTF-8. If the string is empty, then the user canceled the
   action of choosing a file.
   
   Note : because of limitation on MacOS X, and to maintain consistency
   in strongly related standard system windows, no custom customization can
   be added using our Frame or View mechanism. Such customization would
   require a new function that would implement it using standard system
   controls, for each targetted platform.
   
==============================================================================
*/

void  Application::choose_file (std::string & path, const std::string & original_path, const FileFormatList & file_format_list, const std::string & window_title, const std::string & message)
{
   assert (&original_path != 0);
   assert (&window_title != 0);
   assert (&message != 0);
   assert (txt::utf8::Codec8::check_string_valid (original_path) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (window_title) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (message) == txt::Err_OK);
   
   View * view_ptr = ApplicationImpl::use_instance ().get_keyboard_focus_ptr ();
   
   ApplicationImpl::use_instance ().choose_file (
      path,
      original_path,
      file_format_list,
      window_title,
      message
   );

   // deblock after a possible modal loop eating task signals
   
   task::RunLoopGui::use_instance ().notify_post_external_modality ();
   
   // some events might need to be synthetized
   // for example the event might contain a modifier (like command) which
   // may have been released in this call
   
   if (view_ptr != 0)
   {
      ApplicationImpl::use_instance ().synthetize_events (*view_ptr);
   }

   assert (txt::utf8::Codec8::check_string_valid (path) == txt::Err_OK);
}



/*
==============================================================================
Name : choose_files
Description :
   Display a standard file navigation dialog where multiple files maybe
   chosen.
   'original_path' is the path where the navigation dialog should display
   when opened. If the string is empty, then this path is ignored, and
   the default system one is taken.
   'extension_list' is a list of string to match. No wild card, and the
   string match is done only to the end of the string
   'window_title' is the title to show in the dialog window decoration if
   any.
   'message' is the message to display around the file browser if possible.
   On return, 'path_list' is filled with the paths of the chosen files, in
   fopen style paths, UTF-8. If the list is empty, then the user canceled
   the action of choosing files.
   
   Note : because of limitation on MacOS X, and to maintain consistency
   in strongly related standard system windows, no custom customization can
   be added using our Frame or View mechanism. Such customization would
   require a new function that would implement it using standard system
   controls, for each targetted platform.
   
==============================================================================
*/

void  Application::choose_files (std::list <std::string> & path_list, const std::string & original_path, const FileFormatList & file_format_list, const std::string & window_title, const std::string & message)
{
   assert (&original_path != 0);
   assert (&window_title != 0);
   assert (&message != 0);
   assert (txt::utf8::Codec8::check_string_valid (original_path) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (window_title) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (message) == txt::Err_OK);

   View * view_ptr = ApplicationImpl::use_instance ().get_keyboard_focus_ptr ();
   
   ApplicationImpl::use_instance ().choose_files (
      path_list,
      original_path,
      file_format_list,
      window_title,
      message
   );

   // deblock after a possible modal loop eating task signals
   
   task::RunLoopGui::use_instance ().notify_post_external_modality ();

   // some events might need to be synthetized
   // for example the event might contain a modifier (like command) which
   // may have been released in this call
   
   if (view_ptr != 0)
   {
      ApplicationImpl::use_instance ().synthetize_events (*view_ptr);
   }

#if ! defined (NDEBUG)
   for (std::list <std::string>::const_iterator it = path_list.begin ()
   ;  it != path_list.end ()
   ;  ++it)
   {
      assert (txt::utf8::Codec8::check_string_valid (*it) == txt::Err_OK);
   }
#endif
}



/*
==============================================================================
Name : choose_folder
Description :
   Display a standard file navigation dialog where a folder maybe
   chosen.
   'original_path' is the path where the navigation dialog should display
   when opened. If the string is empty, then this path is ignored, and
   the default system one is taken.
   'window_title' is the title to show in the dialog window decoration if
   any.
   'message' is the message to display around the file browser if possible.
   On return, 'path' is filled with the path of the chosen folder, in
   fopen style paths, UTF-8, with a final trailing delimiter. If the path,
   is empty then the user canceled the action of choosing a folder.
   
   Note : because of limitation on MacOS X, and to maintain consistency
   in strongly related standard system windows, no custom customization can
   be added using our Frame or View mechanism. Such customization would
   require a new function that would implement it using standard system
   controls, for each targetted platform.
   
==============================================================================
*/

void  Application::choose_folder (std::string & path, const std::string & original_path, const std::string & window_title, const std::string & message)
{
   assert (&original_path != 0);
   assert (&window_title != 0);
   assert (&message != 0);
   assert (txt::utf8::Codec8::check_string_valid (original_path) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (window_title) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (message) == txt::Err_OK);

   View * view_ptr = ApplicationImpl::use_instance ().get_keyboard_focus_ptr ();
   
   ApplicationImpl::use_instance ().choose_folder (
      path,
      original_path,
      window_title,
      message
   );

   // deblock after a possible modal loop eating task signals
   
   task::RunLoopGui::use_instance ().notify_post_external_modality ();

   // some events might need to be synthetized
   // for example the event might contain a modifier (like command) which
   // may have been released in this call
   
   if (view_ptr != 0)
   {
      ApplicationImpl::use_instance ().synthetize_events (*view_ptr);
   }

   assert (txt::utf8::Codec8::check_string_valid (path) == txt::Err_OK);
}



/*
==============================================================================
Name : choose_volume
Description :
   Display a standard file navigation dialog where a volume (disk) maybe
   chosen.
   'window_title' is the title to show in the dialog window decoration if
   any.
   'message' is the message to display around the file browser if possible.
   On return, 'path' is filled with the path of the chosen volume, in
   fopen style paths, UTF-8, with a final trailing delimiter. If the path,
   is empty then the user canceled the action of choosing a volume.
   
   Note : because of limitation on MacOS X, and to maintain consistency
   in strongly related standard system windows, no custom customization can
   be added using our Frame or View mechanism. Such customization would
   require a new function that would implement it using standard system
   controls, for each targetted platform.
   
==============================================================================
*/

void  Application::choose_volume (std::string & path, const std::string & window_title, const std::string & message)
{
   assert (&window_title != 0);
   assert (&message != 0);
   assert (txt::utf8::Codec8::check_string_valid (window_title) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (message) == txt::Err_OK);

   View * view_ptr = ApplicationImpl::use_instance ().get_keyboard_focus_ptr ();
   
   ApplicationImpl::use_instance ().choose_volume (path, window_title, message);

   // deblock after a possible modal loop eating task signals
   
   task::RunLoopGui::use_instance ().notify_post_external_modality ();

   // some events might need to be synthetized
   // for example the event might contain a modifier (like command) which
   // may have been released in this call
   
   if (view_ptr != 0)
   {
      ApplicationImpl::use_instance ().synthetize_events (*view_ptr);
   }

   assert (txt::utf8::Codec8::check_string_valid (path) == txt::Err_OK);
}



/*
==============================================================================
Name : put_file
Description :
   Display a standard file navigation dialog where a combinaition of file
   name and destination folder may be chosen.
   'original_path' is the path where the navigation dialog should display
   when opened. If the string is empty, then this path is ignored, and
   the default system one is taken.
   'original_filename' is the file name displayed originally in the
   dialog window. This filename contains an extension, and this extension
   is used so that the returned path ends with this extension.
   For example if the original file name is "untitled.txt" and the user
   rename it to "untitled.tx" then the resulting path will end with
   "untitled.tx.txt".
   'message' is the message to display around the file browser if possible.
   On return, 'path' is filled with the path of the combination of chosen
   folder and filename, in fopen style paths, UTF-8. If the string is empty,
   then the user canceled the action of putting a file.
   
   Note : because of limitation on MacOS X, and to maintain consistency
   in strongly related standard system windows, no custom customization can
   be added using our Frame or View mechanism. Such customization would
   require a new function that would implement it using standard system
   controls, for each targetted platform.
   
==============================================================================
*/

void  Application::put_file (std::string & path, const std::string & original_path, const std::string & original_filename, const std::string & window_title, const std::string & message, const std::vector <std::string> &filter)
{
   assert (&original_path != 0);
   assert (&original_filename != 0);
   assert (&window_title != 0);
   assert (&message != 0);
   assert (txt::utf8::Codec8::check_string_valid (original_path) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (original_filename) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (window_title) == txt::Err_OK);
   assert (txt::utf8::Codec8::check_string_valid (message) == txt::Err_OK);

   View * view_ptr = ApplicationImpl::use_instance ().get_keyboard_focus_ptr ();
   
   ApplicationImpl::use_instance ().put_file (
      path,
      original_path,
      original_filename,
      window_title,
      message,
      filter
   );
   
   // deblock after a possible modal loop eating task signals
   
   task::RunLoopGui::use_instance ().notify_post_external_modality ();

   // some events might need to be synthetized
   // for example the event might contain a modifier (like command) which
   // may have been released in this call
   
   if (view_ptr != 0)
   {
      ApplicationImpl::use_instance ().synthetize_events (*view_ptr);
   }
}



/*
==============================================================================
Name : is_matching
==============================================================================
*/

bool  Application::is_matching (const std::string & path, const FileFormatList & file_format_list)
{
   // for now one description matches only one extension
   
   bool ret_val = false;
   
   FileFormatList::const_iterator it = file_format_list.begin ();
   const FileFormatList::const_iterator it_end = file_format_list.end ();
   
   for (; it != it_end ; ++it)
   {
      const FileFormat & file_format = *it;
      
      if (file_format (path))
      {
         ret_val = true;
         
         break;
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : FileFormat::operator ()
==============================================================================
*/

bool  Application::FileFormat::operator () (const std::string & path) const
{
   std::string path_lc;
   
   int err = txt::utf8::conv_to_lower_case (
      path_lc,
      path.c_str (), path.size ()
   );
   assert (err == 0);
   
   if (err != 0)
   {
      ohm_util_TRACE_ERROR1 ("Converting %1% to lower case failed. Ignoring", path.c_str ());
   }
   
   std::list <std::string>::const_iterator it = _extensions.begin ();
   const std::list <std::string>::const_iterator it_end = _extensions.end ();
   
   bool ret_val = false;
   
   for (; it != it_end ; ++it)
   {
      std::string extension = *it;
      extension = "." + extension;
      
      std::string extension_lc;
      
      err = txt::utf8::conv_to_lower_case (
         extension_lc,
         extension.c_str (), extension.size ()
      );
      assert (err == 0);
      
      if (err != 0)
      {
         ohm_util_TRACE_ERROR1 ("Converting %1% to lower case failed. Ignoring", extension.c_str ());
      }
      
      std::string::size_type pos = path_lc.find (extension_lc);
      
      if (pos != std::string::npos)
      {
         if (pos == path_lc.size () - extension_lc.size ())
         {
            ret_val = true;
            
            break;
         }
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : req_signal
==============================================================================
*/

void  Application::req_signal (Event & event)
{
   if (_delegate.is_set ())
   {
      _delegate.param_ref_signal (event);
   }
}



/*
==============================================================================
Name : set_keyboard_focus_target
Description :
   Set target for keyboard pressed msg.
   If the target do not catch the msg, then send the message to the menubar
   If the menubar do not catch the msg, then send the msg to all eligible
   views :
   - views that are contained in the active document window (unique)
   - views that are contained in all floatings windows
   'bnd_ptr' may be 0. In that case, there is no direct target for keyboard
   inputs
==============================================================================
*/

// should be handled by the system ?
// in that case, only views specify focus, which seems normal ?

/*void   Application::set_keyboard_focus_target (MsgHandlerComponent * bnd_ptr)
{
   #warning pay attention to this
   // propagation of keyboard msg to check with the others
   
   // kEventClassTextInput / kEventTextInputUnicodeForKeyEvent
   // kEventParamTextInputSendSLRec (in, typeIntlWritingCode)
   // kEventParamTextInputSendText (in, typeUnicodeText)  array or UniChar (kSpaceCharCode works)
   // typeUTF8Text
   // Text Encoding Conversion Manager Reference
   // Apple Type Services for Fonts Reference
   // http://webcvs.cairographics.org/cairo/src/cairo_atsui_font.c?hideattic=0&revision=1.5&view=markup&sortby=log
   // ATSUDirectGetLayoutDataArrayPtrFromTextLayout
   // ATSUSetTextPointerLocation
   // kEventParamTextInputSendKeyboardEvent to fetch modifiers
   // kEventParamTextInputSendGlyphInfoArray (in, typeGlyphInfoArray)
   // TSM
   // GetUserFocusEventTarget
   
   // UniChar to CFStringRef back to what we want
   // UniChar -> CFStringCreateWithCharacters
   // CFStringGetCString with kCFStringEncodingUTF8
   
   // msdn
   // GetCharacterPlacementW
   
   _keyboard_focus_target_ptr = bnd_ptr;
}*/
   




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Application::Application ()
:  _pasteboard ()
,  _drag_data ()
,  _cursor_stack ()
,  _cursor_stack_invalid_flag (false)
,  _cursor_type (StandardMouseCursor_NONE)
,  _custom_cursor_ptr (0)
,  _cursor_visible_flag (true)
,  _cursor_obscured_flag (false)
#if ! defined (ohm_opal_APPLICATION_FORCE_CURSOR_FLAG)
,  _cursor_ptr (0)
#endif
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
,  _cursor_theme_mac_flag (false)
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
,  _cursor_theme_mac_flag (true)
#endif
{
   ApplicationImpl::use_instance ();
}



/*
==============================================================================
Name : do_process_command
Description :
   process a command.
   The default implementation does nothing and pass the message.
   The propagation of that message is handled one step upper.
==============================================================================
*/

bool  Application::do_process_command (const std::string & command)
{
   return false;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : erase_cursor
==============================================================================
*/

Application::CursorBundle  Application::erase_cursor (void * ptr)
{
   CursorBundle ret_val;
   ret_val._ptr = ptr;
   ret_val._cursor_type = StandardMouseCursor_DEFAULT;
   ret_val._custom_cursor_ptr = 0;
   ret_val._visible_flag = true;
   ret_val._obscured_flag = false;

   std::list <CursorBundle>::iterator it = _cursor_stack.begin ();
   const std::list <CursorBundle>::iterator it_end = _cursor_stack.end ();
   
   for (; it != it_end ;)
   {
      std::list <CursorBundle>::iterator it_next = it;
      ++it_next;
      
      CursorBundle & cursor_bundle = *it;
      
      if (cursor_bundle._ptr == ptr)
      {
         ret_val = cursor_bundle;
         
         _cursor_stack.erase (it);
      }
      
      it = it_next;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : find_cursor
==============================================================================
*/

Application::CursorBundle *   Application::find_cursor (void * ptr)
{
   CursorBundle * ret_val_ptr = 0;

   std::list <CursorBundle>::iterator it = _cursor_stack.begin ();
   const std::list <CursorBundle>::iterator it_end = _cursor_stack.end ();
   
   for (; it != it_end ; ++it)
   {
      CursorBundle & cursor_bundle = *it;
      
      if (cursor_bundle._ptr == ptr)
      {
         ret_val_ptr = &cursor_bundle;
      }
   }
   
   return ret_val_ptr;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
