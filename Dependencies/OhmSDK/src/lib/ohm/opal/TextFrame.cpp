/*****************************************************************************

        TextFrame.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49007

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

#include "ohm/opal/TextFrame.h"
#include "ohm/opa/TextBundle.h"
#include "ohm/opak/TextField.h"

#include "ohm/txt/Err.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/lang/fnc.h"

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
==============================================================================
*/

TextFrame::TextFrame (opa::TextBundle & text_bundle, opak::TextField & text_field)
:  Frame (text_bundle.get_name ())
,  _bundle (text_bundle)
,  _text_field (text_field)
,  _dispatcher (*this)

,  _editing_flag (false)

,  _scripture ()
,  _typesetter_aptr ()
,  _typesetter_need_layout_flag (true)
,  _first_pos (0)
,  _second_pos (0)
{
   using namespace ohm;
   
   _dispatcher._mouse.configure_gesture (
      EventDispatcherStandardMouse <TextFrame>::Button_LEFT,
      EventDispatcherStandardMouse <TextFrame>::ClickCount_ONE,
      &TextFrame::mouse_button_pressed,
      &TextFrame::mouse_button_released,
      &TextFrame::mouse_dragged
   );

   _dispatcher._mouse.configure_mouse_button_pressed (
      Event::Pass_SPATIAL,
      EventDispatcherStandardMouse <TextFrame>::Button_LEFT,
      EventDispatcherStandardMouse <TextFrame>::ClickCount_TWO,
      &TextFrame::mouse_button_pressed_twice
   );

   _dispatcher._mouse.configure_mouse_button_released (
      Event::Pass_SPATIAL,
      EventDispatcherStandardMouse <TextFrame>::Button_LEFT,
      EventDispatcherStandardMouse <TextFrame>::ClickCount_TWO,
      &TextFrame::mouse_button_released_twice
   );

   _dispatcher._keyboard.configure_key (
      Event::Pass_FOCUS,
      &TextFrame::focus_key_pressed,
      &TextFrame::focus_key_repeated,
      &TextFrame::focus_key_released
   );

   _dispatcher._keyboard.configure_characters (
      Event::Pass_FOCUS,
      &TextFrame::focus_characters_typed
   );

   _dispatcher._user_input.configure_lost (
      &TextFrame::user_input_lost
   );
   
   add (_dispatcher);

   focus_lost ();
   
   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
   _typesetter_need_layout_flag = true;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TextFrame::~TextFrame ()
{
}



/*
==============================================================================
Name : push_state_font
==============================================================================
*/

void  TextFrame::push_state_font (const Font & font)
{
   _scripture.push_state_font (font);
}



/*
==============================================================================
Name : clear_state_font
==============================================================================
*/

void  TextFrame::clear_state_font ()
{
   _scripture.clear_state_font ();
}



/*
==============================================================================
Name : set_state_color
==============================================================================
*/

void  TextFrame::set_state_color (const Color & color)
{
   _scripture.set_state_color (color);
}



/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  TextFrame::mouse_button_pressed (Event & event)
{
   if (!_editing_flag)
   {
      begin_edit ();
   }
   else
   {
      opa::Rect rect (opa::Point::_zero, get_bounds ());
      
      bool in_flag = rect.contains (event._mouse._location);
      
      if (in_flag)
      {
         // nothing
      }
      else
      {
         end_edit ();
      }
   }
   
   if (_editing_flag)
   {
      assert (_typesetter_aptr.get () != 0);
      
      update_typesetter ();
      
      _first_pos = _typesetter_aptr->location_to_position (event._mouse._location);
      _second_pos = _first_pos;
      
      _text_field._caret_first = _first_pos;
      _text_field._caret_second = _second_pos;
   }

   event.block ();
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  TextFrame::mouse_button_released (Event & event)
{
   if (_editing_flag)
   {
      assert (_typesetter_aptr.get () != 0);
      
      _second_pos = _typesetter_aptr->location_to_position (event._mouse._location);
      
      _text_field._caret_second = _second_pos;

      animate_caret ();
      
      event.block ();
   }
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  TextFrame::mouse_dragged (Event & event)
{
   if (_editing_flag)
   {
      assert (_typesetter_aptr.get () != 0);
      
      _second_pos = _typesetter_aptr->location_to_position (event._mouse._location);
      
      _text_field._caret_second = _second_pos;
      
      event.block ();
   }
}



/*
==============================================================================
Name : mouse_button_pressed_twice
==============================================================================
*/

void  TextFrame::mouse_button_pressed_twice (Event & event)
{
   if (_editing_flag)
   {
      // to do
      
      event.block ();
   }
}



/*
==============================================================================
Name : mouse_button_released_twice
==============================================================================
*/

void  TextFrame::mouse_button_released_twice (Event & event)
{
   if (_editing_flag)
   {
      // to do
      
      event.block ();
   }
}



/*
==============================================================================
Name : focus_key_pressed
==============================================================================
*/

void  TextFrame::focus_key_pressed (Event & event)
{
   if (_editing_flag)
   {
      bool processed_flag = false;
      
      if (event._key == Key (KeyCode_RETURN))
      {
         erase_between_carets ();
         
         _scripture.begin_edit ();

         _scripture.insert_paragraph_separator (_first_pos);
         
         _first_pos += 1;
         _second_pos = _first_pos;
         
         _scripture.end_edit ();
         
         scripture_changed ();
         
         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_BACKSPACE))
      {
         #pragma message ("not exact")
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
            
            _first_pos = prev_pos;
            _second_pos = prev_pos;
         }

         scripture_changed ();

         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_DEL))
      {
      }
      else if (event._key == Key (KeyCode_LEFT_ARROW) + Key::Shift)
      {
         #pragma message ("not the real thing")
         // we would have to go in glyph order, taking care of
         // ligatures
         
         _second_pos = _scripture.get_previous_cluster_boundary (_second_pos);
         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_LEFT_ARROW))
      {
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
         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_RIGHT_ARROW) + Key::Shift)
      {
         _second_pos = _scripture.get_next_cluster_boundary (_second_pos);
         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_RIGHT_ARROW))
      {
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
         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_DOWN_ARROW))
      {
         // TODO: 
         processed_flag = true;
      }
      else if (event._key == Key (KeyCode_UP_ARROW))
      {
         // TODO: 
         processed_flag = true;
      }

      _text_field._caret_first = _first_pos;
      _text_field._caret_second = _second_pos;
      
      fix_caret ();
      animate_caret ();
      
      if (processed_flag)
      {
         event.block ();
      }
      
      event.skip_passes ();
   }
}



/*
==============================================================================
Name : focus_key_repeated
==============================================================================
*/

void  TextFrame::focus_key_repeated (Event & event)
{
   focus_key_pressed (event);
}



/*
==============================================================================
Name : focus_key_released
==============================================================================
*/

void  TextFrame::focus_key_released (Event & event)
{
   if (_editing_flag)
   {
      event.skip_passes ();
   }
}



/*
==============================================================================
Name : focus_characters_typed
==============================================================================
*/

void  TextFrame::focus_characters_typed (Event & event)
{
   if (_editing_flag)
   {
      {
         ohm::txt::unicode::StringU ustr;
         const std::string & utf8_str = event._str;
         
         {
            int err = txt::Err_OK;
            size_t i = 0;

            while ((err == txt::Err_OK) && (utf8_str [i] != '\0'))
            {
               txt::unicode::Char   ucs4_char;
               int len = 0;

               err = txt::utf8::Codec8::decode_char (ucs4_char, &utf8_str [i], len);
               
               if (err == txt::Err_OK)
               {
                  i += len;
                  ustr += ucs4_char;
               }
            }
         }

         erase_between_carets ();

         _scripture.begin_edit ();

         _scripture.insert (_first_pos, ustr);
         
         _first_pos += ustr.size ();
         _second_pos = _first_pos;
         
         _scripture.end_edit ();
         
         scripture_changed ();
      }

      _text_field._caret_first = _first_pos;
      _text_field._caret_second = _second_pos;

      fix_caret ();
      animate_caret ();
      
      event.block ();
   }
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  TextFrame::user_input_lost (Event & event)
{
   if (_editing_flag)
   {
      assert (_typesetter_aptr.get () != 0);
      
      _second_pos = _typesetter_aptr->location_to_position (event._mouse._location);
      
      _text_field._caret_second = _second_pos;
      
      end_edit ();
   }
}



/*
==============================================================================
Name : keyboard_focus_lost
==============================================================================
*/

void  TextFrame::keyboard_focus_lost (const Event & event)
{
   focus_lost ();
   
   _editing_flag = false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : begin_edit
==============================================================================
*/

void  TextFrame::begin_edit ()
{
   acquire_mouse_capture ();
   acquire_keyboard_focus ();
   
   focus_gain ();
   fix_caret ();
   
   _editing_flag = true;
}



/*
==============================================================================
Name : end_edit
==============================================================================
*/

void  TextFrame::end_edit ()
{
   release_mouse_capture ();
   release_keyboard_focus ();
   
   // focus_lost & _editing_flag will be called in 'keyboard_focus_lost'
}



/*
==============================================================================
Name : scripture_changed
==============================================================================
*/

void  TextFrame::scripture_changed ()
{
   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
   _typesetter_need_layout_flag = true;
   
   _text_field._scripture = _scripture;
}



/*
==============================================================================
Name : update_typesetter
==============================================================================
*/

void  TextFrame::update_typesetter ()
{
   assert (_typesetter_aptr.get () != 0);

   if (_typesetter_need_layout_flag)
   {
      const float width = get_bounds ()._width;

      Typesetter & typesetter (*_typesetter_aptr);
      
      size_t pos = 0;
         
      const Typesetter::LineInfo * last_line_info_ptr = 0;
      
      for (; pos < _scripture.size () ;)
      {
         size_t next_pos = 0;
         float height = 0;
         typesetter.suggest_line_break (next_pos, height, pos, width);
         
         Scripture::Range range;
         range._start = pos;
         range._end = next_pos;

         const Typesetter::LineInfo * line_info_ptr
            = typesetter.layout_line (range, width, false, last_line_info_ptr);
         
         last_line_info_ptr = line_info_ptr;
         
         pos = next_pos;
      }
      
      _typesetter_need_layout_flag = false;
   }
}



/*
==============================================================================
Name : erase_between_carets
==============================================================================
*/

void  TextFrame::erase_between_carets ()
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
      
      _first_pos = first_pos;
      _second_pos = first_pos;
   }
}



/*
==============================================================================
Name : fix_caret
==============================================================================
*/

void  TextFrame::fix_caret ()
{
   _text_field._caret_color = opal::Color (1.f, 0.f, 0.f, 0.f);
}



/*
==============================================================================
Name : animate_caret
==============================================================================
*/

void  TextFrame::animate_caret ()
{
   _text_field._caret_color
      .stop ()
      .push (opal::Color (1.f, 0.f, 0.f, 0.f), .5f, opak::step)
      .push (opal::Color (0.f, 0.f, 0.f, 0.f), .5f, opak::step)
      .loop ();
}



/*
==============================================================================
Name : focus_lost
==============================================================================
*/

void  TextFrame::focus_lost ()
{
   _text_field._caret_color = opal::Color (0.f, 0.f, 0.f, 0.f);
   _text_field._selection_color = opal::Color (.25f, .25f, .25f, .25f);
}



/*
==============================================================================
Name : focus_gain
==============================================================================
*/

void  TextFrame::focus_gain ()
{
   _text_field._selection_color = opal::Color (.5f, .25f, 0.f, 1.f);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
