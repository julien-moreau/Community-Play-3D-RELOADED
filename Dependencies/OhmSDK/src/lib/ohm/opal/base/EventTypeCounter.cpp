/*****************************************************************************

        EventTypeCounter.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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

#include "ohm/opal/base/EventTypeCounter.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

EventTypeCounter::EventTypeCounter ()
{
   for (size_t i = 0 ; i < Base_NBR_ELT ; ++i)
   {
      _type_cnt_arr [i] = 0;
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

EventTypeCounter::~EventTypeCounter ()
{
}



/*
==============================================================================
Name : empty
Description :
   Returns true if 'this' does not support any event, false otherwise.
==============================================================================
*/

bool  EventTypeCounter::empty () const
{
   bool empty_flag = true;
   
   for (size_t i = 0 ; i < Base_NBR_ELT ; ++i)
   {
      size_t type_cnt = _type_cnt_arr [i];
      
      if (type_cnt > 0)
      {
         empty_flag = false;
         break;
      }
   }
   
   return empty_flag;
}



/*
==============================================================================
Name : does_support
Description :
   Returns true if the frame with which it is associated *might* handle the
   event.
   Returns false if the frame with which it is associated *never* handle the
   event.
Note :
   Internally, 'this' will encode efficiently a number of combination entered
   through all the 'add_*' function, but not all multiplicity is actually
   singurarly encoded : large rare plane may be encoded at once, like, for
   example, all button for which number is greater than 2 : in practice
   we don't use them often, but a particular client of this library might
   change this class for its particular needs, if performance dictates so.
==============================================================================
*/

bool  EventTypeCounter::does_support (const Event & event) const
{
   int pass_plane = get_pass_plane (event._pass);
   int button_nbr_plane = get_button_nbr_plane (event);
   int click_count_plane = get_click_count_plane (event._mouse._click_count);
   
   size_t pos = 0;
   
   switch (event._type)
   {
   case Event::Type_MOUSE_MOVED:
      pos
         = Base_MOUSE_MOVED
         + pass_plane * Metrics_MOVED_NBR_BUTTON
         + button_nbr_plane;
      break;

   case Event::Type_MOUSE_BUTTON_PRESSED:
   case Event::Type_MOUSE_BUTTON_RELEASED:
      pos
         = Base_MOUSE_BUTTON_CHANGED
         + pass_plane * Metrics_BUT_CHD_NBR_BUTTON * Metrics_BUT_CHD_NBR_CLICK
         + button_nbr_plane * Metrics_BUT_CHD_NBR_CLICK
         + click_count_plane;
      break;

   case Event::Type_MOUSE_WHEEL_MOVED:
      pos = Base_MOUSE_WHEEL_MOVED + pass_plane;
      break;

   case Event::Type_MOUSE_CATCH_CAPTURE:
      pos = Base_CATCH_CAPTURE;
      break;

   case Event::Type_MOUSE_CURSOR:
      pos = Base_CURSOR;
      break;

   case Event::Type_KEY_PRESSED:
      pos = Base_KEY_PRESSED + pass_plane;
      break;

   case Event::Type_KEY_REPEATED:
      pos = Base_KEY_REPEATED + pass_plane;
      break;

   case Event::Type_KEY_RELEASED:
      pos = Base_KEY_RELEASED + pass_plane;
      break;

   case Event::Type_CHARACTERS_TYPED:
      pos = Base_CHARACTER_TYPED + pass_plane;
      break;

   case Event::Type_KEYBOARD_CATCH_FOCUS:
      pos = Base_KEYBOARD_CATCH_FOCUS;
      break;

   case Event::Type_USER_INPUT_LOST:
      pos = Base_USER_INPUT_LOST;
      break;

   case Event::Type_CONDUCT_CHANGED:
      pos = Base_CONDUCT_CHANGED + pass_plane;
      break;
   
   case Event::Type_CUSTOM_SENT:
      pos = Base_CUSTOM_SENT + pass_plane;
      break;
   
   case Event::Type_DATA_DROPPED:
      pos = Base_DATA_DROPPED + pass_plane;
      break;
   
   case Event::Type_DEBUG:
      pos = Base_DEBUG;
      break;
   
   default:
      assert (false);
      break;
   }
   
   assert (pos < Base_NBR_ELT);
   
   size_t type_cnt = _type_cnt_arr [pos];
   
   return type_cnt > 0;
}
   


/*
==============================================================================
Name : increment
==============================================================================
*/

void  EventTypeCounter::increment (const EventTypeCounter & other)
{
   for (size_t i = 0 ; i < Base_NBR_ELT ; ++i)
   {
      _type_cnt_arr [i] += other._type_cnt_arr [i];
   }
}



/*
==============================================================================
Name : decrement
==============================================================================
*/

void  EventTypeCounter::decrement (const EventTypeCounter & other)
{
   for (size_t i = 0 ; i < Base_NBR_ELT ; ++i)
   {
      size_t & type_cnt = _type_cnt_arr [i];
      size_t other_type_cnt = other._type_cnt_arr [i];
      
      assert (type_cnt >= other_type_cnt);
      
      type_cnt -= other_type_cnt;
   }
}



/*
==============================================================================
Name : add_all
==============================================================================
*/

void  EventTypeCounter::add_all ()
{
   for (size_t i = 0 ; i < Base_NBR_ELT ; ++i)
   {
      _type_cnt_arr [i] += 1;
   }
}



/*
==============================================================================
Name : add_mouse_moved
==============================================================================
*/

void  EventTypeCounter::add_mouse_moved (Event::Pass pass, int button_nbr)
{
   int pass_plane = get_pass_plane (pass);
   int button_nbr_plane = (button_nbr == 0) ? 0 : 1;

   size_t pos
      = Base_MOUSE_MOVED
      + pass_plane * Metrics_MOVED_NBR_BUTTON
      + button_nbr_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_mouse_button_state_changed
==============================================================================
*/

void  EventTypeCounter::add_mouse_button_state_changed (Event::Pass pass, int button_nbr, int click_count)
{
   int pass_plane = get_pass_plane (pass);
   int button_nbr_plane = get_button_nbr_plane (button_nbr);
   int click_count_plane = get_click_count_plane (click_count);

   size_t pos
      = Base_MOUSE_BUTTON_CHANGED
      + pass_plane * Metrics_BUT_CHD_NBR_BUTTON * Metrics_BUT_CHD_NBR_CLICK
      + button_nbr_plane * Metrics_BUT_CHD_NBR_CLICK
      + click_count_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_mouse_wheel_moved
==============================================================================
*/

void  EventTypeCounter::add_mouse_wheel_moved (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_MOUSE_WHEEL_MOVED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_mouse_catch_capture
==============================================================================
*/

void  EventTypeCounter::add_mouse_catch_capture ()
{
   ++_type_cnt_arr [Base_CATCH_CAPTURE];
}



/*
==============================================================================
Name : add_mouse_cursor
==============================================================================
*/

void  EventTypeCounter::add_mouse_cursor ()
{
   ++_type_cnt_arr [Base_CURSOR];
}



/*
==============================================================================
Name : add_key_pressed
==============================================================================
*/

void  EventTypeCounter::add_key_pressed (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_KEY_PRESSED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_key_repeated
==============================================================================
*/

void  EventTypeCounter::add_key_repeated (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_KEY_REPEATED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_key_released
==============================================================================
*/

void  EventTypeCounter::add_key_released (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_KEY_RELEASED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_character_typed
==============================================================================
*/

void  EventTypeCounter::add_character_typed (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_CHARACTER_TYPED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_keyboard_catch_focus
==============================================================================
*/

void  EventTypeCounter::add_keyboard_catch_focus ()
{
   ++_type_cnt_arr [Base_KEYBOARD_CATCH_FOCUS];
}



/*
==============================================================================
Name : add_user_input_lost
==============================================================================
*/

void  EventTypeCounter::add_user_input_lost ()
{
   ++_type_cnt_arr [Base_USER_INPUT_LOST];
}



/*
==============================================================================
Name : add_conduct_changed
==============================================================================
*/

void  EventTypeCounter::add_conduct_changed (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_CONDUCT_CHANGED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_custom_sent
==============================================================================
*/

void  EventTypeCounter::add_custom_sent (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_CUSTOM_SENT + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_data_dropped
==============================================================================
*/

void  EventTypeCounter::add_data_dropped (Event::Pass pass)
{
   int pass_plane = get_pass_plane (pass);
   
   size_t pos = Base_DATA_DROPPED + pass_plane;
   
   ++_type_cnt_arr [pos];
}



/*
==============================================================================
Name : add_debug
==============================================================================
*/

void  EventTypeCounter::add_debug ()
{
   ++_type_cnt_arr [Base_DEBUG];
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
