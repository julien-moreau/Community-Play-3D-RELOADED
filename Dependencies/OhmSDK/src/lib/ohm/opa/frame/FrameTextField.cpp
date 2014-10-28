/*****************************************************************************

        FrameTextField.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71208

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/frame/FrameTextField.h"

#include "ohm/archi/def.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/Cursor.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/FontManager.h"
#include "ohm/txt/utf8/def.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/unicode/unicode.h"
#include "ohm/txt/Err.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Event::ctor
==============================================================================
*/

FrameTextField::Event::Event (FrameTextField & frame)
:  _frame (frame)
,  _height (frame.get_height ())
,  _first_caret_pos (frame.get_first_caret_pos ())
,  _second_caret_pos (frame.get_second_caret_pos ())
{
}



/*
==============================================================================
Name : Config::ctor
==============================================================================
*/

FrameTextField::Config::Config ()
:  _commit_on_outside_click_flag (true)
,  _commit_on_tab_key_flag (true)
,  _end_edit_on_commit_flag (true)
,  _edit_on_double_click_flag (false)
,  _allow_copy_to_clipboard_flag (true)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

FrameTextField::FrameTextField (opal::Scripture & scripture, opal::Scripture * processed_scripture_ptr)
:  opal::Frame ("FrameTextField")
,  _scripture (scripture)
,  _processed_scripture_ptr (processed_scripture_ptr)
,  _dispatcher (*this)

,  _mode (Mode_FIXED)
,  _editing_flag (false)
,  _typesetter_aptr ()
,  _typesetter_need_layout_flag (true)
,  _height (0.f)
,  _first_pos (0)
,  _second_pos (0)
,  _tracking_flag (false)
,  _char_proc (0)
,  _delegate ()
{
   _dispatcher._keyboard.configure_characters (
      opal::Event::Pass_FOCUS,
      &FrameTextField::focus_characters_typed
   );
   
   _dispatcher._keyboard.configure_key (
      opal::Event::Pass_FOCUS,
      &FrameTextField::focus_key_pressed,
      &FrameTextField::focus_key_released
   );
   
   _dispatcher._keyboard.configure_catch_focus (
      &FrameTextField::catch_focus
   );
   
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_ONE,
      &FrameTextField::mouse_button_pressed,
      &FrameTextField::mouse_button_released,
      &FrameTextField::mouse_dragged
   );
   
   _dispatcher._mouse.configure_mouse_button_pressed (
      opal::Event::Pass_FOCUS,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_ONE,
      &FrameTextField::mouse_button_pressed
   );

   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_FOCUS,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_ONE,
      &FrameTextField::mouse_button_released
   );

   _dispatcher._mouse.configure_mouse_dragged (
      opal::Event::Pass_FOCUS,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_ONE,
      &FrameTextField::mouse_dragged
   );

   _dispatcher._mouse.configure_mouse_button_pressed (
      opal::Event::Pass_FOCUS,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_TWO,
      &FrameTextField::mouse_button_pressed_twice
   );

   _dispatcher._mouse.configure_mouse_button_pressed (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_TWO,
      &FrameTextField::mouse_button_pressed_twice
   );

   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_FOCUS,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_TWO,
      &FrameTextField::mouse_button_released_twice
   );
   
   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameTextField>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameTextField>::ClickCount_TWO,
      &FrameTextField::mouse_button_released_twice
   );
   
   _dispatcher._mouse.configure_mouse_moved (
      opal::Event::Pass_FOCUS,
      &FrameTextField::mouse_moved
   );

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   _dispatcher._mouse.configure_cursor (
      &FrameTextField::mouse_cursor
   );
#endif

   _dispatcher._user_input.configure_lost (
      &FrameTextField::user_input_lost
   );
   
   add (_dispatcher);
   
   if (_processed_scripture_ptr != 0)
   {
      _typesetter_aptr = std::auto_ptr <opal::Typesetter> (
         new opal::Typesetter (*_processed_scripture_ptr)
      );
   }
   else
   {
      _typesetter_aptr = std::auto_ptr <opal::Typesetter> (
         new opal::Typesetter (_scripture)
      );
   }
   
   _typesetter_need_layout_flag = true;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameTextField::~FrameTextField ()
{
}



/*
==============================================================================
Name : add_key_bind_commit
==============================================================================
*/

void  FrameTextField::add_key_bind_commit (const opal::Key & key)
{
   _key_map_flag [key] = true;
}



/*
==============================================================================
Name : add_key_bind_revert
==============================================================================
*/

void  FrameTextField::add_key_bind_revert (const opal::Key & key)
{
   _key_map_flag [key] = false;
}



/*
==============================================================================
Name: set_mode
==============================================================================
*/

void  FrameTextField::set_mode (Mode mode)
{
   _mode = mode;
}



/*
==============================================================================
Name : get_config
==============================================================================
*/

const FrameTextField::Config &   FrameTextField::get_config () const
{
   return _config;
}



/*
==============================================================================
Name : set_config
==============================================================================
*/

void  FrameTextField::set_config (const Config & config)
{
   _config = config;
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float FrameTextField::get_height () const
{
   return _height;
}



/*
==============================================================================
Name : get_first_caret_pos
==============================================================================
*/

size_t   FrameTextField::get_first_caret_pos () const
{
   return _first_pos;
}



/*
==============================================================================
Name : get_second_caret_pos
==============================================================================
*/

size_t   FrameTextField::get_second_caret_pos () const
{
   return _second_pos;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  FrameTextField::set (const std::string & text)
{
   txt::unicode::StringU ustr;
   int err = txt::unicode::conv_utf8_to_unicode (ustr, text.c_str ());
   assert (err == txt::Err_OK);

   txt::unicode::StringU proc_ustr;
   
   if (_char_proc != 0)
   {
      txt::unicode::StringU::size_type pos = 0;
      
      for (; pos < ustr.size () ; ++pos)
      {
         txt::unicode::Char proc_char = _char_proc (ustr [pos]);
         
         proc_ustr.push_back (proc_char);
      }
   }
   else
   {
      proc_ustr = ustr;
   }

   _scripture.begin_edit ();
   _scripture.clear ();
   _scripture.insert (0, ustr);
   _scripture.end_edit ();
   
   if (_processed_scripture_ptr != 0)
   {
      _processed_scripture_ptr->begin_edit ();
      _processed_scripture_ptr->clear ();
      _processed_scripture_ptr->insert (0, proc_ustr);
      _processed_scripture_ptr->end_edit ();
   }

   _first_pos = ustr.size ();
   _second_pos = _first_pos;
      
   scripture_changed ();
}
   


/*
==============================================================================
Name : set_char_proc
==============================================================================
*/

void  FrameTextField::set_char_proc (CharProc char_proc)
{
   _char_proc = char_proc;
}


   
/*
==============================================================================
Name: select_all
==============================================================================
*/

void  FrameTextField::select_all ()
{
   if (_scripture.size () > 0)
   {
      _first_pos = 0;
      _second_pos = _scripture.size ();
      
      carets_positions_changed ();
   }
}



/*
==============================================================================
Name: start_edit
==============================================================================
*/

void  FrameTextField::start_edit ()
{
   if (!_editing_flag)
   {
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
      opal::Application::use_instance ().set_cursor_ibeam ();
#else
      set_cursor ("ohm.cursor.ibeam.png", opa::Point (3, 8));
#endif
      
      take_keyboard_focus ();
      
      // this will trigger a 'begin_edit' in 'catch_focus'
   }
}



/*
==============================================================================
Name: cancel_edit
==============================================================================
*/

void  FrameTextField::cancel_edit ()
{
   if (_editing_flag)
   {
      focus_lost ();
   }
}



/*
==============================================================================
Name: deselect_all
==============================================================================
*/

void  FrameTextField::deselect_all ()
{
   _first_pos = _scripture.size ();
   _second_pos = _first_pos;
   
   carets_positions_changed ();
}


/*
==============================================================================
Name : handle_backspace
==============================================================================
*/

void  FrameTextField::handle_backspace ()
{
   // normally we would check to see if we can just
   // delete the last char without breaking a cluster validity
   // for that we just look if _pos - 1 and _pos + 1 form a cluster
   // boundary
   // if not we would delete from prev_pos to _pos
   
   if (_first_pos != _second_pos)
   {
      erase_between_carets ();
   }
   else
   {
      size_t prev_pos = _scripture.get_previous_cluster_boundary (_first_pos);
      
      _scripture.begin_edit ();
      _scripture.erase (prev_pos, _first_pos);
      _scripture.end_edit ();
      
      if (_processed_scripture_ptr != 0)
      {
         // is this ok ?
         _processed_scripture_ptr->begin_edit ();
         _processed_scripture_ptr->erase (prev_pos, _first_pos);
         _processed_scripture_ptr->end_edit ();
      }
      
      _first_pos = prev_pos;
      _second_pos = prev_pos;
   }
   
   scripture_changed ();
}



/*
==============================================================================
Name : handle_suppr
==============================================================================
*/

void  FrameTextField::handle_suppr ()
{
   if (_first_pos != _second_pos)
   {
      erase_between_carets ();
   }
   else
   {
      size_t next_pos = _scripture.get_next_cluster_boundary (_first_pos);
      
      _scripture.begin_edit ();
      _scripture.erase (_first_pos, next_pos);
      _scripture.end_edit ();
      
      if (_processed_scripture_ptr != 0)
      {
         // is this ok ?
         _processed_scripture_ptr->begin_edit ();
         _processed_scripture_ptr->erase (_first_pos, next_pos);
         _processed_scripture_ptr->end_edit ();
      }
      
//    _first_pos = next_pos;
      _second_pos = _first_pos;
   }
   
   scripture_changed ();
}



/*
==============================================================================
Name : handle_left_move
==============================================================================
*/

void  FrameTextField::handle_left_move ()
{
   // we would have to go in glyph order, taking care of
   // ligatures

   if (_first_pos != _second_pos)
   {
      if (_first_pos < _second_pos)
      {
         _second_pos = _first_pos;
      }
      else
      {
         _first_pos = _second_pos;
      }
   }
   else
   {
      _first_pos = _scripture.get_previous_cluster_boundary (_first_pos);
      _second_pos = _first_pos;
   }
   
   carets_positions_changed ();
}



/*
==============================================================================
Name : handle_right_move
==============================================================================
*/

void  FrameTextField::handle_right_move ()
{
   // we would have to go in glyph order, taking care of
   // ligatures

   if (_first_pos != _second_pos)
   {
      if (_first_pos < _second_pos)
      {
         _first_pos = _second_pos;
      }
      else
      {
         _second_pos = _first_pos;
      }
   }
   else
   {
      _first_pos = _scripture.get_next_cluster_boundary (_first_pos);
      _second_pos = _first_pos;
   }
   
   carets_positions_changed ();
}


/*
==============================================================================
Name: handle_home_move
==============================================================================
*/

void  FrameTextField::handle_home_move ()
{
   _first_pos = 0;
   _second_pos = _first_pos;
   
   carets_positions_changed ();
}



/*
==============================================================================
Name: handle_end_move
==============================================================================
*/

void  FrameTextField::handle_end_move ()
{
   _first_pos = _scripture.size ();
   _second_pos = _first_pos;
   
   carets_positions_changed ();
}


/*
==============================================================================
Name : handle_left_select
==============================================================================
*/

void  FrameTextField::handle_left_select ()
{
   _second_pos = _scripture.get_previous_cluster_boundary (_second_pos);
   
   carets_positions_changed ();
}



/*
==============================================================================
Name : handle_right_select
==============================================================================
*/

void  FrameTextField::handle_right_select ()
{
   _second_pos = _scripture.get_next_cluster_boundary (_second_pos);
   
   carets_positions_changed ();
}


/*
==============================================================================
Name: handle_home_select
==============================================================================
*/

void  FrameTextField::handle_home_select ()
{
   _second_pos = 0;
   
   carets_positions_changed ();
}



/*
==============================================================================
Name: handle_end_select
==============================================================================
*/

void  FrameTextField::handle_end_select ()
{
   _second_pos = _scripture.size ();
   
   carets_positions_changed ();
}


/*
==============================================================================
Name : handle_characters
==============================================================================
*/

void  FrameTextField::handle_characters (const std::string & str)
{
   txt::unicode::StringU ustr;
   int err = txt::unicode::conv_utf8_to_unicode (ustr, str.c_str ());
   assert (err == txt::Err_OK);
   
   erase_between_carets ();
   
   txt::unicode::StringU proc_ustr;
   
   if (_char_proc != 0)
   {
      txt::unicode::StringU::size_type pos = 0;
      
      for (; pos < ustr.size () ; ++pos)
      {
         txt::unicode::Char proc_char = _char_proc (ustr [pos]);
         
         proc_ustr.push_back (proc_char);
      }
   }
   else
   {
      proc_ustr = ustr;
   }
   
   _scripture.begin_edit ();
   _scripture.insert (_first_pos, ustr);
   _scripture.end_edit ();
   
   if (_processed_scripture_ptr != 0)
   {
      _processed_scripture_ptr->begin_edit ();
      _processed_scripture_ptr->insert (_first_pos, proc_ustr);
      _processed_scripture_ptr->end_edit ();
   }
   
   _first_pos += ustr.size ();
   _second_pos = _first_pos;
   
   
   scripture_changed ();
}



/*
==============================================================================
Name : focus_key_pressed
==============================================================================
*/

void  FrameTextField::focus_key_pressed (opal::Event & event)
{
   assert (_editing_flag);
   
   obscure_cursor ();
   
   bool block_event_flag = true;
   
   KeyFlagMap::iterator it = _key_map_flag.find (event._key);
   
   if (it != _key_map_flag.end ())
   {
      bool commit_flag = it->second;
      
      Event frame_event (*this);
      frame_event._type = commit_flag ? Event::Type_COMMIT : Event::Type_REVERT;
      frame_event._key = event._key;
      
      if (_config._end_edit_on_commit_flag)
      {
         end_edit ();
      }
      
      _delegate.param_ref_signal (frame_event);
   }
   else
   {
      switch (event._key.get_key_code ())
      {
         case opal::KeyCode_TAB:
            {
               if (_config._commit_on_tab_key_flag)
               {
                  Event frame_event (*this);
                  frame_event._type = Event::Type_COMMIT;
                  
                  end_edit ();
                  
                  _delegate.param_ref_signal (frame_event);
               }
               else
               {
                  end_edit ();
               }
               
               if (event._key.has_shift_modifier ())
               {
                  previous_keyboard_focus ();
               }
               else
               {
                  next_keyboard_focus ();
               }
            }
            break;
            
         case opal::KeyCode_BACKSPACE:
            handle_backspace ();
            break;
            
         case opal::KeyCode_DEL:
            handle_suppr ();
            break;
            
         case opal::KeyCode_LEFT_ARROW:
            if (event._key.has_shift_modifier ())
            {
               handle_left_select ();
            }
            else
            {
               handle_left_move ();
            }
            break;
            
         case opal::KeyCode_RIGHT_ARROW:
            if (event._key.has_shift_modifier ())
            {
               handle_right_select ();
            }
            else
            {
               handle_right_move ();
            }
            break;
            
            
         case opal::KeyCode_UP_ARROW:
         case opal::KeyCode_HOME:
            if (event._key.has_shift_modifier ())
            {
               handle_home_select ();
            }
            else
            {
               handle_home_move ();
            }
            break;
            
         case opal::KeyCode_DOWN_ARROW:
         case opal::KeyCode_END:
            if (event._key.has_shift_modifier ())
            {
               handle_end_select ();
            }
            else
            {
               handle_end_move ();
            }
            break;
            
         default:
            block_event_flag = false;
            break;
      };
   }
   
   if (block_event_flag)
   {
      event.block ();
   }
   
   event.skip_passes ();
}



/*
==============================================================================
Name : focus_key_released
==============================================================================
*/

void  FrameTextField::focus_key_released (opal::Event & event)
{
   assert (_editing_flag);
   
   event.skip_passes ();
}



/*
==============================================================================
Name : focus_characters_typed
==============================================================================
*/

void  FrameTextField::focus_characters_typed (opal::Event & event)
{
   assert (_editing_flag);

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)  
   bool characters_flag = true;
   
   // handling AltGr
   if (event._key.has_command_modifier ())
   {
      characters_flag = event._key.has_alt_modifier ();
   }

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   bool characters_flag = !event._key.has_command_modifier ();

#endif
   
   if (characters_flag)
   {
      handle_characters (event._str);
      
      event.block ();
   }
   
   event.skip_passes ();
}



/*
==============================================================================
Name : catch_focus
==============================================================================
*/

void  FrameTextField::catch_focus (opal::Event & event)
{
   focus_gain ();
   
   event.block ();
}



/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FrameTextField::mouse_button_pressed (opal::Event & event)
{
   bool begin_edit_flag = false;
   bool end_edit_flag = false;
   
   if (!_editing_flag)
   {
      if (!_config._edit_on_double_click_flag)
      {
         begin_edit ();
         
         begin_edit_flag = true;
      }
   }
   else
   {
      // mouse is captured
      
      opa::Rect rect (opa::Point::_zero, get_bounds ());
   
      bool in_flag = rect.contains (event._mouse._location);
      
      if (in_flag)
      {
         // nothing
      }
      else
      {
         if (_config._commit_on_outside_click_flag)
         {
            Event frame_event (*this);
            frame_event._type = Event::Type_COMMIT;
            
            end_edit ();
         
            _delegate.param_ref_signal (frame_event);
         }
         else
         {
            end_edit ();
         }
         
         end_edit_flag = true;
      }
   }
   
   if (begin_edit_flag)
   {
      select_all ();
      
      event.block ();
   }
   else if (_editing_flag)
   {
      update_typesetter ();

      Point mouse_location = event._mouse._location;
      
      _first_pos = _typesetter_aptr->location_to_position (event._mouse._location);
      _second_pos = _first_pos;
      
      carets_positions_changed ();
      
      _tracking_flag = true;
      
      event.block ();
   }
   else if (end_edit_flag)
   {
      // nothing, don't block event
   }
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  FrameTextField::mouse_button_released (opal::Event & event)
{  
   if (_tracking_flag)
   {
      assert (_editing_flag);
      
      Point mouse_location = event._mouse._location;
      
      _second_pos = _typesetter_aptr->location_to_position (mouse_location);
      
      carets_positions_changed ();
      
      _tracking_flag = false;

      event.block ();
   }
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FrameTextField::mouse_dragged (opal::Event & event)
{
   if (_tracking_flag)
   {
      assert (_editing_flag);
      
      Point mouse_location = event._mouse._location;
      
      _second_pos = _typesetter_aptr->location_to_position (mouse_location);
      
      carets_positions_changed ();

      event.block ();
   }
}



/*
==============================================================================
Name: mouse_button_pressed_twice
==============================================================================
*/

void  FrameTextField::mouse_button_pressed_twice (opal::Event & event)
{
   if (_editing_flag)
   {
      select_all ();
      
      event.block ();
   }
   else if (_config._edit_on_double_click_flag)
   {
      begin_edit ();
      
      select_all ();
      
      event.block ();
   }
}



/*
==============================================================================
Name: mouse_button_released_twice
==============================================================================
*/

void  FrameTextField::mouse_button_released_twice (opal::Event & event)
{
   if (_editing_flag)
   {
      event.block ();
   }
   
   _tracking_flag = false;
}



/*
==============================================================================
Name: mouse_dragged_twice
==============================================================================
*/

void  FrameTextField::mouse_dragged_twice (opal::Event & event)
{
   if (_editing_flag)
   {
      event.block ();
   }
}



/*
==============================================================================
Name: mouse_moved
==============================================================================
*/

void  FrameTextField::mouse_moved (opal::Event & event)
{
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   if (_editing_flag)
   {
      opa::Rect rect (opa::Point::_zero, get_bounds ());
      
      bool in_flag = rect.contains (event._mouse._location);
      
      if (in_flag)
      {
         opal::Application::use_instance ().set_cursor_ibeam ();
      }
      else
      {
         opal::Application::use_instance ().set_cursor_arrow ();
      }
   }
   
#else
   opa::Rect rect (opa::Point::_zero, get_bounds ());
   
   if (rect.contains (event._mouse._location))
   {
      set_cursor ("ohm.cursor.ibeam.png", opa::Point (3, 8));
   }
   else
   {
      release_cursor ();
   }
#endif
}



/*
==============================================================================
Name: mouse_cursor
==============================================================================
*/

void  FrameTextField::mouse_cursor (opal::Event & event)
{
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   bool ibeam_flag = true;
   
   if (_config._edit_on_double_click_flag)
   {
      ibeam_flag = false;
   }
   
   opa::Rect rect (opa::Point::_zero, get_bounds ());
   ibeam_flag &= rect.contains (event._mouse._location);
   
   if (ibeam_flag)
   {
      opal::Application::use_instance ().set_cursor_ibeam ();
      
      event.block ();
   }
#endif
}



/*
==============================================================================
Name: user_input_lost
==============================================================================
*/

void  FrameTextField::user_input_lost (opal::Event & event)
{
   focus_lost ();
}



/*
==============================================================================
Name: mouse_entered
==============================================================================
*/

void  FrameTextField::mouse_entered (opal::Event & event)
{
   if (_editing_flag || !_config._edit_on_double_click_flag)
   {
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
      set_cursor ("ohm.cursor.ibeam.png", opa::Point (3, 8));
#endif
   }

   Event fw_event (*this);
   fw_event._type = Event::Type_ENTERED;
   
   _delegate.param_ref_signal (fw_event);
}



/*
==============================================================================
Name: mouse_exited
==============================================================================
*/

void  FrameTextField::mouse_exited (opal::Event & event)
{
   if (_editing_flag || !_config._edit_on_double_click_flag)
   {
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
      release_cursor ();
#endif
   }

   Event fw_event (*this);
   fw_event._type = Event::Type_EXITED;
   
   _delegate.param_ref_signal (fw_event);
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  FrameTextField::mouse_target_lost (const opal::Event & event)
{
   bool notify_flag = _editing_flag;
   
   _editing_flag = false;
   
   focus_lost ();
   
   if (notify_flag)
   {
      Event event (*this);
      event._type = Event::Type_END;
      
      _delegate.param_ref_signal (event);
   }
}



/*
==============================================================================
Name : keyboard_focus_lost
==============================================================================
*/

void  FrameTextField::keyboard_focus_lost (const opal::Event & event)
{
   bool notify_flag = _editing_flag;
   
   _editing_flag = false;
   
   focus_lost ();
   
   if (notify_flag)
   {
      Event event (*this);
      event._type = Event::Type_END;
      
      _delegate.param_ref_signal (event);
   }
}



/*
==============================================================================
Name : req_menu_cut
==============================================================================
*/

void  FrameTextField::req_menu_cut (opal::MenuEvent & event)
{
   if (! _config._allow_copy_to_clipboard_flag)
   {
      return;
   }
   
   req_menu_copy (event);
   
   if (_first_pos != _second_pos)
   {
      erase_between_carets ();
   }
}



/*
==============================================================================
Name : req_menu_copy
==============================================================================
*/

void  FrameTextField::req_menu_copy (opal::MenuEvent & event)
{
   if (! _config._allow_copy_to_clipboard_flag)
   {
      return;
   }
   
   if (_first_pos != _second_pos)
   {
      size_t first_pos = _first_pos;
      size_t second_pos = _second_pos;
      
      if (first_pos > second_pos)
      {
         std::swap (first_pos, second_pos);
      }
      
      std::string str;
      
      char utf8_0 [txt::utf8::MAX_BYTE_SEQ_LEN + 1];
      
      for (size_t i = first_pos ; i < second_pos ; ++i)
      {
         const txt::unicode::Char uc = _scripture.get_char (i);
         
         if (txt::utf8::Codec8::encode_char (utf8_0, uc) == 0)
         {
            str += utf8_0;
         }
      }
      
      opal::Data pasteboard_data;
      pasteboard_data._mime_type_data_map_arr.resize (1);
      
      std::vector <archi::UByte> & data
         = pasteboard_data._mime_type_data_map_arr.back ()[opal::Data::_mime_text_plain_0];
      
      data.assign (str.begin (), str.end ());
      
      opal::Application::use_instance ().set_pasteboard (pasteboard_data);
      
      event._handled_flag = true;
   }
}



/*
==============================================================================
Name : req_menu_paste
==============================================================================
*/

void  FrameTextField::req_menu_paste (opal::MenuEvent & event)
{
   opal::Data pasteboard_data;
   
   opal::Application::use_instance ().get_pasteboard (pasteboard_data);
   
   if (!pasteboard_data._mime_type_data_map_arr.empty ())
   {
      if (
         pasteboard_data._mime_type_data_map_arr.back ().find (opal::Data::_mime_text_plain_0)
         != pasteboard_data._mime_type_data_map_arr.back ().end ()
         )
      {
         const std::vector <archi::UByte> & data
            = pasteboard_data._mime_type_data_map_arr.back () [opal::Data::_mime_text_plain_0];
         
         std::string str;
         str.assign (data.begin (), data.end ());
         
         handle_characters (str);
      }
   }
   
   event._handled_flag = true;
}



/*
==============================================================================
Name : req_menu_select_all
==============================================================================
*/

void  FrameTextField::req_menu_select_all (opal::MenuEvent & event)
{
   select_all ();
   
   event._handled_flag = true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
/*
==============================================================================
Name : begin_edit
==============================================================================
*/

void  FrameTextField::begin_edit ()
{
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   opal::Application::use_instance ().set_cursor_ibeam ();
#else
   set_cursor ("ohm.cursor.ibeam.png", opa::Point (3, 8));
#endif
   
   acquire_keyboard_focus ();
   acquire_mouse_capture ();
   
   _editing_flag = true;

   focus_gain ();
   
   _menu_link_cut.bind <
      FrameTextField,
      &FrameTextField::req_menu_cut
   > (*this, opal::MenuStdId_CUT);

   _menu_link_copy.bind <
      FrameTextField,
      &FrameTextField::req_menu_copy
   > (*this, opal::MenuStdId_COPY);

   _menu_link_paste.bind <
      FrameTextField,
      &FrameTextField::req_menu_paste
   > (*this, opal::MenuStdId_PASTE);

   _menu_link_select_all.bind <
      FrameTextField,
      &FrameTextField::req_menu_select_all
   > (*this, opal::MenuStdId_SELECT_ALL);

   Event event (*this);
   event._type = Event::Type_BEGIN;
   
   _delegate.param_ref_signal (event);
}



/*
==============================================================================
Name : end_edit
==============================================================================
*/

void  FrameTextField::end_edit ()
{
   _editing_flag = false;
   
   _menu_link_cut.unbind ();
   _menu_link_copy.unbind ();
   _menu_link_paste.unbind ();
   _menu_link_select_all.unbind ();
   
   release_keyboard_focus ();
   release_mouse_capture ();
   
   // focus_lost will be called in 'keyboard_focus_lost'

   Event event (*this);
   event._type = Event::Type_END;
   
   _delegate.param_ref_signal (event);
   
   Event fw_event (*this);
   fw_event._type = Event::Type_EXITED;
      
   _delegate.param_ref_signal (fw_event);
}



/*
==============================================================================
Name : focus_lost
==============================================================================
*/

void  FrameTextField::focus_lost ()
{
   bool notify_flag = _editing_flag;
   
   release_keyboard_focus ();
   release_mouse_capture ();
   
   _editing_flag = false;
   
   deselect_all ();

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   release_cursor ();
#endif
   
   if (notify_flag)
   {
      Event event (*this);
      event._type = Event::Type_REVERT;
      
      _delegate.param_ref_signal (event);
   }
}



/*
==============================================================================
Name : focus_gain
==============================================================================
*/

void  FrameTextField::focus_gain ()
{
   if (!_editing_flag)
   {
      begin_edit ();
   }
   
   select_all ();
}



/*
==============================================================================
Name : scripture_changed
==============================================================================
*/

void  FrameTextField::scripture_changed ()
{
   if (_processed_scripture_ptr != 0)
   {
      _typesetter_aptr = std::auto_ptr <opal::Typesetter> (
         new opal::Typesetter (*_processed_scripture_ptr)
      );
   }
   else
   {
      _typesetter_aptr = std::auto_ptr <opal::Typesetter> (
         new opal::Typesetter (_scripture)
      );
   }

   _typesetter_need_layout_flag = true;
   
   Event event (*this);
   event._type = Event::Type_SCRIPTURE_CHANGED;
   
   _delegate.param_ref_signal (event);
   
   if (_mode != Mode_FIXED)
   {
      update_typesetter ();
   }
}



/*
==============================================================================
Name : carets_positions_changed
==============================================================================
*/

void  FrameTextField::carets_positions_changed ()
{
   Event event (*this);
   event._type = Event::Type_CARETS_POSITIONS_CHANGED;
   
   _delegate.param_ref_signal (event);
}



/*
==============================================================================
Name : update_typesetter
==============================================================================
*/

void  FrameTextField::update_typesetter ()
{
   assert (_typesetter_aptr.get () != 0);

   if (_typesetter_need_layout_flag)
   {
      float width = get_bounds ()._width;
      
      opal::Typesetter & typesetter (*_typesetter_aptr);
      
      size_t pos = 0;
         
      const opal::Typesetter::LineInfo * last_line_info_ptr = 0;
      
      const size_t size
         = (_processed_scripture_ptr != 0)
         ? _processed_scripture_ptr->size ()
         : _scripture.size ()
      ;
      
      float height = 0;
      
      for (; pos < size ;)
      {
         size_t next_pos = 0;
         float sub_height = 0;
         typesetter.suggest_line_break (next_pos, sub_height, pos, width);
         
         height += sub_height;
         
         opal::Scripture::Range range; 
         range._start = pos;
         range._end = next_pos;

         const opal::Typesetter::LineInfo * line_info_ptr
            = typesetter.layout_line (range, width, false, last_line_info_ptr);
         
         last_line_info_ptr = line_info_ptr;
         
         pos = next_pos;
      }
      
      if ((_height != height) && (_mode == Mode_FIXED_WIDTH))
      {
         _height = height;
         
         Event event (*this);
         event._type = Event::Type_HEIGHT_CHANGED;
         
         _delegate.param_ref_signal (event);
      }
      
      _typesetter_need_layout_flag = false;
   }
}



/*
==============================================================================
Name : erase_between_carets
==============================================================================
*/

void  FrameTextField::erase_between_carets ()
{
   if (_first_pos != _second_pos)
   {
      size_t first_pos = _first_pos;
      size_t second_pos = _second_pos;
      
      if (first_pos > second_pos)
      {
         std::swap (first_pos, second_pos);
      }
      
      _scripture.begin_edit ();
      _scripture.erase (first_pos, second_pos);
      _scripture.end_edit ();
      
      if (_processed_scripture_ptr != 0)
      {
         _processed_scripture_ptr->begin_edit ();
         _processed_scripture_ptr->erase (first_pos, second_pos);
         _processed_scripture_ptr->end_edit ();
      }
      
      _first_pos = first_pos;
      _second_pos = first_pos;

      scripture_changed ();
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

