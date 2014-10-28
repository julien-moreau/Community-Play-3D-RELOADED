/*****************************************************************************

        SystemEventMgr.cpp
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 67167

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

#include "ohm/opal/private/SystemEventMgr.h"
#include "ohm/opal/View.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: SystemEventMgr
==============================================================================
*/

SystemEventMgr::SystemEventMgr (View & view)
:  _view (view)
{
}



/*
==============================================================================
Name: ~SystemEventMgr
==============================================================================
*/

SystemEventMgr::~SystemEventMgr ()
{
}



/*
==============================================================================
Name: process
==============================================================================
*/

bool SystemEventMgr::process (SystemEvent & event)
{
   bool key_catched_flag = false;
   
   while (_event != event)
   {
      // do stuff
      
      if (_event._mouse_location != event._mouse_location)
      {
         // check ViewImpl::_inhibit delta_flag
         
         opa::Point delta = event._mouse_location - _event._mouse_location;
         
         _view.mouse_moved (event._mouse_location, delta);
         
         _event._mouse_location = event._mouse_location;
      }
      else if (_event._click_cnt_arr [SystemEvent::MouseButton_L] != event._click_cnt_arr [SystemEvent::MouseButton_L])
      {
         handle_button_event (SystemEvent::MouseButton_L, event);
      }
      else if (_event._click_cnt_arr [SystemEvent::MouseButton_R] != event._click_cnt_arr [SystemEvent::MouseButton_R])
      {
         handle_button_event (SystemEvent::MouseButton_R, event);
      }
      else if (_event._key_change != event._key_change)
      {
         key_catched_flag |= handle_key_change (event);
      }
      else if (_event._key != event._key)
      {
         // if we have changed not reported by key_change, handle it here
         
         key_catched_flag |= handle_key (event);
      }
      else if (_event._char_utf8 != event._char_utf8)
      {
         key_catched_flag |= _view.characters_typed (event._char_utf8);
         
         _event._char_utf8 = event._char_utf8;
      }
      else if (_event._wheel_delta != event._wheel_delta)
      {
         _view.mouse_wheel_moved (event._wheel_delta);
         
         _event._wheel_delta = event._wheel_delta;
      }
      else if (_event._layout_changed_flag != event._layout_changed_flag)
      {
         assert (event._layout_changed_flag);
         
         _view.view_layout_changed ();
         
         _event._layout_changed_flag = event._layout_changed_flag;
      }
      else if (_event._has_drag_data_flag != event._has_drag_data_flag)
      {
         assert (event._has_drag_data_flag);
         
         _view.mouse_moved (event._mouse_location, opa::Point::_zero);
         
         _event._has_drag_data_flag = event._has_drag_data_flag;
      }
      else if (_event._drag_data_dropped_flag != event._drag_data_dropped_flag)
      {
         assert (event._drag_data_dropped_flag);
         
         _view.mouse_data_dropped (event._mouse_location, opa::Point::_zero);
         
         _event._drag_data_dropped_flag = event._drag_data_dropped_flag;
      }
      else if (_event._mouse_exited_view_flag != event._mouse_exited_view_flag)
      {
         assert (event._mouse_exited_view_flag);
         
         _view.mouse_exited_view (event._mouse_location, opa::Point::_zero);
         
         _event._mouse_exited_view_flag = event._mouse_exited_view_flag;
      }
      else if (_event._keyboard_focus_lost_flag != event._keyboard_focus_lost_flag)
      {
         assert (event._keyboard_focus_lost_flag);

         _view.keyboard_focus_lost ();
         
         _event._keyboard_focus_lost_flag = event._keyboard_focus_lost_flag;
      }
      else
      {
         assert (false);
      }
   }
   
   _event.reset_transient ();
   event.reset_transient ();
   
   return key_catched_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: handle_button_event
==============================================================================
*/

void SystemEventMgr::handle_button_event (SystemEvent::MouseButton but_bit, const SystemEvent & event)
{
   assert (_event._click_cnt_arr [but_bit] != event._click_cnt_arr [but_bit]);
   assert (_event._click_cnt_arr [but_bit] >= 0);
   assert ( event._click_cnt_arr [but_bit] >= 0);

   // check there is a release between each click
   assert (
      (_event._click_cnt_arr [but_bit] == 0) || 
      (event._click_cnt_arr [but_bit] == 0)
   );
   
   // button nbr start from 1 (see EventDispatcherStandardMouse::Button)
   const int opal_button = but_bit + 1;
   
   if (event._click_cnt_arr [but_bit] != 0)
   {
      _view.mouse_moved (_event._mouse_location, opa::Point::_zero);

      _view.mouse_button_pressed (opal_button, event._click_cnt_arr [but_bit]); 
   }
   else
   {
      _view.mouse_button_released (opal_button); 
   }
   
   _event._click_cnt_arr [but_bit] = event._click_cnt_arr [but_bit];
}



/*
==============================================================================
Name: handle_key_change
==============================================================================
*/

bool SystemEventMgr::handle_key_change (const SystemEvent & event)
{
   assert (_event._key_change != event._key_change);
   
   bool catched_flag = false;
   
   switch (event._key_change)
   {
   case SystemEvent::KeyChange_PRESSED:
      catched_flag = _view.key_pressed (event._key);
      break;
         
   case SystemEvent::KeyChange_REPEATED:
      catched_flag = _view.key_repeated (event._key);
      break;
         
   case SystemEvent::KeyChange_RELEASED:
      catched_flag = _view.key_released (event._key);
      break;
   
   default:
      assert (false);
      break;
   }
   
   _event._key = event._key;
   _event._key_change = event._key_change;
   
   return catched_flag;
}



/*
==============================================================================
Name: handle_key
==============================================================================
*/

bool SystemEventMgr::handle_key (const SystemEvent & event)
{
   assert (_event._key != event._key);
   assert (_event._key_change == event._key_change);
   
   bool catched_flag = false;
   
   if (_event._key._shift_flag != event._key._shift_flag)
   {
      _event._key._shift_flag = event._key._shift_flag;
      
      if (event._key._shift_flag)
      {
         _view.key_pressed (event._key);
      }
      else
      {
         _view.key_released (event._key);
      }
   }
   
   if (_event._key._alt_flag != event._key._alt_flag)
   {
      _event._key._alt_flag = event._key._alt_flag;
      
      if (event._key._alt_flag)
      {
         _view.key_pressed (event._key);
      }
      else
      {
         _view.key_released (event._key);
      }
   }

   if (_event._key._command_flag != event._key._command_flag)
   {
      _event._key._command_flag = event._key._command_flag;
      
      if (event._key._command_flag)
      {
         _view.key_pressed (event._key);
      }
      else
      {
         _view.key_released (event._key);
      }
   }
   
   if (
      (_event._key._key_code != event._key._key_code)
      || (_event._key._key_char != event._key._key_char)
      )
   {
      if (_event._key != Key::None)
      {
         // don't update _key
         
         _view.key_released (event._key);
      }
      
      _event._key = event._key;
      
      if (_event._key != Key::None)
      {
         // don't update _key
         
         _view.key_pressed (event._key);
      }
   }

   _event._key = event._key;
   
   return catched_flag;
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

