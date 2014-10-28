/*****************************************************************************

        Application.h
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



#if ! defined (ohm_opal_Application_HEADER_INCLUDED)
#define  ohm_opal_Application_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/opal/def.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/StandardMouseCursor.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/HIDEvent.h"
#include "ohm/opa/Point.h"
#include "ohm/util/Delegate.h"

#include <list>
#include <set>
#include <string>

#define ohm_opal_APPLICATION_FORCE_CURSOR_FLAG



namespace ohm
{
namespace opal
{



class Data;
class Window;
/*class QuitListener;*/
class Cursor;

class Application
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      enum Type
      {
                  Type_HID_EVENT_ARR = 0,
      };
      
      Type        _type;
      std::vector <HIDEvent>
                  _hid_event_arr;
   };
   
   class FileFormat
   {
   public:
                  FileFormat (const std::string & description) : _description (description) {}
      std::string _description;
      std::list <std::string>
                  _extensions;
      
      bool        operator () (const std::string & path) const;
   };
   typedef std::list <FileFormat> FileFormatList;

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)   
   enum StdCursor
   {
                  StdCursor_ARROW = 0,
                  StdCursor_ARROW_POPUP,
                  StdCursor_HAND_POINTING,
                  StdCursor_IBEAM,
                  StdCursor_RESIZE_VERTICAL,
                  
   };
#endif
   
   virtual        ~Application ();
   
   static void    create ();

   static Application &
                  use_instance ();
               
   void           run ();
   bool           request_quit ();
   
   template <class Receiver, void (Receiver::*handle_event) (Event &)>
   void           bind (Receiver & receiver);
   
   void           set_ui_mode_windowed ();
   void           set_ui_mode_immersive ();
   
   void           set_pasteboard (const Data & data);
   void           get_pasteboard (Data & data);
   
   void           process_command (const std::string & command);

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           set (opal::Cursor & cursor);
   void           set (StdCursor cursor);
   void           set_cursor_arrow ();
   void           set_cursor_arrow_popup ();
   void           set_cursor_hand_pointing ();
   void           set_cursor_ibeam ();
   void           set_cursor_resize_vertical ();
   void           set_cursor_theme_mac ();
   void           set_cursor_theme_win ();
   void           set_cursor (const std::string & path, const opa::Point & hot_pt);
#else
   void           set_cursor (void * ptr, int depth, StandardMouseCursor cursor);
   void           set_cursor (void * ptr, int depth, opal::Cursor & cursor);
   void           release_cursor (void * ptr);
#endif
   void           show_cursor (void * ptr, bool show_flag);
   void           obscure_cursor (void * ptr);
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           apply_cursor ();  // internal
#endif
   
   void           choose_file (std::string & path, const std::string & original_path, bool (*filter_proc) (const char * /*path_0*/), const std::string & window_title, const std::string & message);

   void           choose_file (std::string & path, const std::string & original_path, const FileFormatList & file_format_list, const std::string & window_title, const std::string & message);
   void           choose_files (std::list <std::string> & path_list, const std::string & original_path, const FileFormatList & file_format_list, const std::string & window_title, const std::string & message);

   void           choose_folder (std::string & path, const std::string & original_path, const std::string & window_title, const std::string & message);
   void           choose_volume (std::string & path, const std::string & window_title, const std::string & message);

   void           put_file (std::string & path, const std::string & original_path, const std::string & original_filename, const std::string & window_title, const std::string & message, const std::vector <std::string> &filter);

   bool           is_matching (const std::string & path, const FileFormatList & file_format_list);
   
   // internal
   void           req_signal (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  Application ();
   
   static Application &
                  use_instance (Application * app_ptr);
   
   virtual bool   do_process_command (const std::string & command);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class CursorBundle
   {
   public:
      bool        operator < (const CursorBundle & other) const
      {
         return _depth < other._depth;
      }
      
      void *      _ptr;
      int         _depth;
      StandardMouseCursor
                  _cursor_type;
      Cursor *    _custom_cursor_ptr;
      bool        _visible_flag;
      bool        _obscured_flag;
   };
   
   CursorBundle   erase_cursor (void * ptr);
   CursorBundle * find_cursor (void * ptr);
   
   Data           _pasteboard;
   Data           _drag_data;
   
   std::list <CursorBundle>
                  _cursor_stack;
   bool           _cursor_stack_invalid_flag;
   StandardMouseCursor
                  _cursor_type;
   Cursor *       _custom_cursor_ptr;
   bool           _cursor_visible_flag;
   bool           _cursor_obscured_flag;

#if ! defined (ohm_opal_APPLICATION_FORCE_CURSOR_FLAG)   
   Cursor *       _cursor_ptr;
#endif
   
   bool           _cursor_theme_mac_flag;
   
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Application (const Application &other);
   Application &  operator = (const Application &other);
   bool           operator == (const Application &other);
   bool           operator != (const Application &other);

}; // class Application



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Application.hpp"



#endif   // ohm_opal_Application_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
