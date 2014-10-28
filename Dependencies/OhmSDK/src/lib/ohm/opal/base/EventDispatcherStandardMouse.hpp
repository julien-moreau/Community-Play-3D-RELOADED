/*****************************************************************************

        EventDispatcherStandardMouse.hpp
        Copyright (c) 2009 Ohm Force

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



#if defined (ohm_opal_EventDispatcherStandardMouse_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventDispatcherStandardMouse code header.
#endif
#define  ohm_opal_EventDispatcherStandardMouse_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventDispatcherStandardMouse_CODEHEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardMouse_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"



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

template <class T>
EventDispatcherStandardMouse <T>::EventDispatcherStandardMouse (T & object)
:  _object (object)
,  _max_nbr_click_arr ()

,  _mouse_button_pressed_proc_arr ()
,  _mouse_button_released_proc_arr ()
,  _mouse_dragged_proc_arr ()
,  _mouse_moved_proc_arr ()
,  _mouse_wheel_moved_proc_arr ()
,  _mouse_catch_capture_proc (0)
,  _mouse_cursor_proc (0)
{
   assert (&object != 0);
      
   for (size_t i = 0 ; i < Button_NBR_ELT ; ++i)
   {
      _max_nbr_click_arr [i] = 0;
   }
   
   ohm_lang_CHECK_CST (Event_Pass_FOCUS_is_0, Event::Pass_FOCUS == 0);
   ohm_lang_CHECK_CST (Event_Pass_SPATIAL_is_1, Event::Pass_SPATIAL == 1);
   
   for (size_t p = 0 ; p < Pass_NBR_ELT ; ++p)
   {
      for (size_t i = 0 ; i < Button_NBR_ELT ; ++i)
      {
         for (size_t j = 0 ; j < ClickCount_NBR_ELT ; ++j)
         {
            _mouse_button_pressed_proc_arr [p][i][j] = 0;
            _mouse_button_released_proc_arr [p][i][j] = 0;
            _mouse_dragged_proc_arr [p][i][j] = 0;
         }
      }
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EventDispatcherStandardMouse <T>::~EventDispatcherStandardMouse ()
{
}



/*
==============================================================================
Name : configure_extend_click_count
Description :
   To be used when the object configure a click count of one, but do not want
   to get called on other click count.
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_extend_click_count (Button button, ClickCount count)
{
   assert (button >= 1);
   assert (button <= Button_NBR_ELT);

   assert (count >= 1);
   assert (count <= ClickCount_NBR_ELT);

   int button_nbr = button - 1;

   lang::max_in_place (
      _max_nbr_click_arr [button_nbr], int (count)
   );
}



/*
==============================================================================
Name : configure_gesture
Description :
   Configure a gesture (pressed, dragged, released) which assumes a
   mouse capture. That is pressed is associated to Pass_SPATIAL, and
   dragged & released are associated to Pass_FOCUS.
   
   When used alone, this gesture will be called for any click count.
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_gesture (Button button, Proc mouse_button_pressed_proc, Proc mouse_button_released_proc, Proc mouse_dragged_proc)
{
   configure_gesture (
      button, ClickCount_ONE,
      mouse_button_pressed_proc, mouse_button_released_proc, mouse_dragged_proc
   );
}



/*
==============================================================================
Name : configure_gesture
Description :
   Configure a gesture (pressed, dragged, released) which assumes a
   mouse capture. That is pressed is associated to Pass_SPATIAL, and
   dragged & released are associated to Pass_FOCUS.
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_gesture (Button button, ClickCount count, Proc mouse_button_pressed_proc, Proc mouse_button_released_proc, Proc mouse_dragged_proc)
{
   assert (button >= 1);
   assert (button <= Button_NBR_ELT);

   assert (count >= 1);
   assert (count <= ClickCount_NBR_ELT);
   
   int button_nbr = button - 1;
   int count_nbr = count - 1;
   
   lang::max_in_place (
      _max_nbr_click_arr [button_nbr], int (count)
   );
   
   _mouse_button_pressed_proc_arr [Event::Pass_SPATIAL][button_nbr][count_nbr]
      = mouse_button_pressed_proc;

   _mouse_button_released_proc_arr [Event::Pass_FOCUS][button_nbr][count_nbr]
      = mouse_button_released_proc;

   _mouse_dragged_proc_arr [Event::Pass_FOCUS][button_nbr][count_nbr]
      = mouse_dragged_proc;
}



/*
==============================================================================
Name : configure_mouse_button_pressed
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_mouse_button_pressed (Event::Pass pass, Button button, ClickCount count, Proc mouse_button_pressed_proc)
{
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   assert (button >= 1);
   assert (button <= Button_NBR_ELT);

   assert (count >= 1);
   assert (count <= ClickCount_NBR_ELT);

   int button_nbr = button - 1;
   int count_nbr = count - 1;
   
   lang::max_in_place (
      _max_nbr_click_arr [button_nbr], int (count)
   );
   
   _mouse_button_pressed_proc_arr [pass][button_nbr][count_nbr]
      = mouse_button_pressed_proc;
}



/*
==============================================================================
Name : configure_mouse_button_released
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_mouse_button_released (Event::Pass pass, Button button, ClickCount count, Proc mouse_button_released_proc)
{
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   assert (button >= 1);
   assert (button <= Button_NBR_ELT);

   assert (count >= 1);
   assert (count <= ClickCount_NBR_ELT);

   int button_nbr = button - 1;
   int count_nbr = count - 1;
   
   lang::max_in_place (
      _max_nbr_click_arr [button_nbr], int (count)
   );
   
   _mouse_button_released_proc_arr [pass][button_nbr][count_nbr]
      = mouse_button_released_proc;
}



/*
==============================================================================
Name : configure_mouse_dragged
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_mouse_dragged (Event::Pass pass, Button button, ClickCount count, Proc mouse_dragged_proc)
{
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   assert (button >= 1);
   assert (button <= Button_NBR_ELT);

   assert (count >= 1);
   assert (count <= ClickCount_NBR_ELT);
   
   int button_nbr = button - 1;
   int count_nbr = count - 1;
   
   lang::max_in_place (
      _max_nbr_click_arr [button_nbr], int (count)
   );
   
   _mouse_dragged_proc_arr [pass][button_nbr][count_nbr]
      = mouse_dragged_proc;
}



/*
==============================================================================
Name : configure_mouse_moved
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_mouse_moved (Proc mouse_moved_proc)
{
   configure_mouse_moved (Event::Pass_SPATIAL, mouse_moved_proc);
}



/*
==============================================================================
Name : configure_mouse_moved
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_mouse_moved (Event::Pass pass, Proc mouse_moved_proc)
{
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   _mouse_moved_proc_arr [pass]
      = mouse_moved_proc;
}



/*
==============================================================================
Name : configure_mouse_moved
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_wheel (Proc mouse_wheel_moved_proc)
{
   configure_wheel (Event::Pass_SPATIAL, mouse_wheel_moved_proc);
}



/*
==============================================================================
Name : configure_wheel
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_wheel (Event::Pass pass, Proc mouse_wheel_moved_proc)
{
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   _mouse_wheel_moved_proc_arr [pass]
      = mouse_wheel_moved_proc;
}



/*
==============================================================================
Name : configure_catch_capture
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_catch_capture (Proc mouse_catch_capture_proc)
{
   _mouse_catch_capture_proc = mouse_catch_capture_proc;
}



/*
==============================================================================
Name : configure_cursor
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::configure_cursor (Proc mouse_cursor_proc)
{
   _mouse_cursor_proc = mouse_cursor_proc;
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::get_cnt (EventTypeCounter & counter)
{
   for (size_t p = 0 ; p < Pass_NBR_ELT ; ++p)
   {
      for (size_t i = 0 ; i < Button_NBR_ELT ; ++i)
      {
         int max_nbr_click = _max_nbr_click_arr [i];
         
         for (size_t j = 0 ; j < ClickCount_NBR_ELT ; ++j)
         {
            int click_count = j;
            
            if (max_nbr_click > 0)
            {
               click_count = j % max_nbr_click;
            }
            
            if (_mouse_button_pressed_proc_arr [p][i][click_count] != 0)
            {
               counter.add_mouse_button_state_changed (
                  static_cast <Event::Pass> (p), i + 1, j + 1
               );
            }

            if (_mouse_button_released_proc_arr [p][i][click_count] != 0)
            {
               counter.add_mouse_button_state_changed (
                  static_cast <Event::Pass> (p), i + 1, j + 1
               );
            }

            if (_mouse_dragged_proc_arr [p][i][click_count] != 0)
            {
               counter.add_mouse_moved (static_cast <Event::Pass> (p), i + 1);
            }
         }
      }
      
      if (_mouse_moved_proc_arr [p] != 0)
      {
         counter.add_mouse_moved (static_cast <Event::Pass> (p), 0);
      }
      
      if (_mouse_wheel_moved_proc_arr [p] != 0)
      {
         counter.add_mouse_wheel_moved (static_cast <Event::Pass> (p));
      }
   }
   
   if (_mouse_catch_capture_proc != 0)
   {
      counter.add_mouse_catch_capture ();
   }

   if (_mouse_cursor_proc != 0)
   {
      counter.add_mouse_cursor ();
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::dispatch (Event & event)
{
   if (event._pass > Event::Pass_SPATIAL)
   {
      return;
   }
   
   switch (event._type)
   {
   case Event::Type_MOUSE_MOVED:
      if (event._mouse._pressed_flag)
      {
         call_dragged (event);
      }
      else
      {
         call_moved (event);
      }
      break;

   case Event::Type_MOUSE_BUTTON_PRESSED:
      call_pressed (event);
      break;

   case Event::Type_MOUSE_BUTTON_RELEASED:
      call_released (event);
      break;

   case Event::Type_MOUSE_WHEEL_MOVED:
      call_wheel_moved (event);
      break;
   
   case Event::Type_MOUSE_CATCH_CAPTURE:
      if (_mouse_catch_capture_proc != 0)
      {
         (_object.*_mouse_catch_capture_proc) (event);
      }
      break;
   
   case Event::Type_MOUSE_CURSOR:
      if (_mouse_cursor_proc != 0)
      {
         (_object.*_mouse_cursor_proc) (event);
      }
      break;
   
   default:
      assert (false);
      break;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : call_pressed
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::call_pressed (Event & event)
{
   int pass = event._pass;
   
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);

   bool ok_flag = true;
      
   int button_nbr = event._mouse._button_nbr;
   
   ok_flag &= button_nbr >= 1;
   ok_flag &= button_nbr <= Button_NBR_ELT;
   
   if (ok_flag)
   {
      int max_nbr_click = _max_nbr_click_arr [button_nbr - 1];
      
      if (max_nbr_click > 0)
      {
         int click_count = event._mouse._click_count;
         
         click_count = ((click_count - 1) % max_nbr_click) + 1;
         
         ok_flag &= click_count >= 1;
         ok_flag &= click_count <= ClickCount_NBR_ELT;

         if (ok_flag)
         {
            Proc proc = _mouse_button_pressed_proc_arr [pass][button_nbr - 1][click_count - 1];
            
            if (proc != 0)
            {
               (_object.*proc) (event);
            }
         }
      }
   }
}



/*
==============================================================================
Name : call_released
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::call_released (Event & event)
{
   int pass = event._pass;
   
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);

   bool ok_flag = true;
      
   int button_nbr = event._mouse._button_nbr;
   
   ok_flag &= button_nbr >= 1;
   ok_flag &= button_nbr <= Button_NBR_ELT;
   
   if (ok_flag)
   {
      int max_nbr_click = _max_nbr_click_arr [button_nbr - 1];
      
      if (max_nbr_click > 0)
      {
         int click_count = event._mouse._click_count;
         int button_nbr = event._mouse._button_nbr;

         click_count = ((click_count - 1) % max_nbr_click) + 1;
         
         bool ok_flag = true;
         
         ok_flag &= click_count >= 1;
         ok_flag &= click_count <= ClickCount_NBR_ELT;

         ok_flag &= button_nbr >= 1;
         ok_flag &= button_nbr <= Button_NBR_ELT;
         
         if (ok_flag)
         {
            Proc proc = _mouse_button_released_proc_arr [pass][button_nbr - 1][click_count - 1];
            
            if (proc != 0)
            {
               (_object.*proc) (event);
            }
         }
      }
   }
}



/*
==============================================================================
Name : call_dragged
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::call_dragged (Event & event)
{
   int pass = event._pass;
   
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);

   bool ok_flag = true;
      
   int button_nbr = event._mouse._button_nbr;
   
   ok_flag &= button_nbr >= 1;
   ok_flag &= button_nbr <= Button_NBR_ELT;
   
   if (ok_flag)
   {
      int max_nbr_click = _max_nbr_click_arr [button_nbr - 1];
      
      if (max_nbr_click > 0)
      {
         int click_count = event._mouse._click_count;
         int button_nbr = event._mouse._button_nbr;

         click_count = ((click_count - 1) % max_nbr_click) + 1;
         
         bool ok_flag = true;
         
         ok_flag &= click_count >= 1;
         ok_flag &= click_count <= ClickCount_NBR_ELT;

         ok_flag &= button_nbr >= 1;
         ok_flag &= button_nbr <= Button_NBR_ELT;
         
         if (ok_flag)
         {
            Proc proc = _mouse_dragged_proc_arr [pass][button_nbr - 1][click_count - 1];
            
            if (proc != 0)
            {
               (_object.*proc) (event);
            }
         }
      }
   }
}



/*
==============================================================================
Name : call_moved
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::call_moved (Event & event)
{
   int pass = event._pass;
   
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   Proc proc = _mouse_moved_proc_arr [pass];
            
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



/*
==============================================================================
Name : call_wheel_moved
==============================================================================
*/

template <class T>
void  EventDispatcherStandardMouse <T>::call_wheel_moved (Event & event)
{
   int pass = event._pass;
   
   assert (pass >= 0);
   assert (pass <= Event::Pass_SPATIAL);
   
   Proc proc = _mouse_wheel_moved_proc_arr [pass];
            
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_EventDispatcherStandardMouse_CODEHEADER_INCLUDED

#undef ohm_opal_EventDispatcherStandardMouse_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
