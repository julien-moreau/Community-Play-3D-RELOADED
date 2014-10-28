/*****************************************************************************

        FrameMusicTime.cpp
        Copyright (c) 2012 Ohm Force

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

#include "ohm/opa/frame/FrameMusicTime.h"
#include "ohm/opa/WidgetMusicTime.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/Cursor.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/opal/def.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: FrameMusicTime
==============================================================================
*/

FrameMusicTime::FrameMusicTime (WidgetMusicTime & widget, int field)
:  opal::Frame ("ctlb::FrameMusicTime")
,  _widget (widget)
,  _field (field)
,  _dispatcher (*this)
,  _tracker_value (_widget, *this, field)
{
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameMusicTime>::Button_LEFT,
      &FrameMusicTime::mouse_button_pressed,
      &FrameMusicTime::mouse_button_released,
      &FrameMusicTime::mouse_dragged
   );

   _dispatcher._mouse.configure_mouse_button_pressed (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameMusicTime>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameMusicTime>::ClickCount_TWO,
      &FrameMusicTime::mouse_button_pressed_twice
   );

   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameMusicTime>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameMusicTime>::ClickCount_TWO,
      &FrameMusicTime::mouse_button_released_twice
   );
      
   _dispatcher._mouse.configure_mouse_moved (
      &FrameMusicTime::mouse_moved
   );

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   _dispatcher._mouse.configure_cursor (
      &FrameMusicTime::mouse_cursor
   );
#endif
   
   _dispatcher._user_input.configure_lost (
      &FrameMusicTime::user_input_lost
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name: ~FrameMusicTime
==============================================================================
*/

FrameMusicTime::~FrameMusicTime ()
{
}



/*
==============================================================================
Name : mouse_entered
==============================================================================
*/

void  FrameMusicTime::mouse_entered (opal::Event & event)
{
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   set_cursor ("ohm.cursor.resize.vertical.png", opa::Point (8, 8));
   
   propagate_children (event);
#endif
}



/*
==============================================================================
Name : mouse_exited
==============================================================================
*/

void  FrameMusicTime::mouse_exited (opal::Event & event)
{
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   propagate_children (event);
   
   release_cursor ();
#endif
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  FrameMusicTime::mouse_target_lost (const opal::Event & event)
{
   _tracker_value.mouse_target_lost ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : mouse_moved
==============================================================================
*/

void  FrameMusicTime::mouse_moved (opal::Event & event)
{
   // nothing. Workaround needed for now to have mouse_entered/exited () 
   // working corretly.
}



/*
==============================================================================
Name : mouse_cursor
==============================================================================
*/

void  FrameMusicTime::mouse_cursor (opal::Event & event)
{
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   opal::Application::use_instance ().set_cursor_resize_vertical ();
   
   event.block ();
#endif
}



/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FrameMusicTime::mouse_button_pressed (opal::Event & event)
{
   _tracker_value.start (event);
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  FrameMusicTime::mouse_button_released (opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_value.is_tracking ();
   
   if (tracking_flag)
   {
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
      opa::Rect rect (opa::Point::_zero, get_bounds ());
   
      if (rect.contains (event._mouse._location))
      {
         set_cursor ("ohm.cursor.resize.vertical.png", opa::Point (8, 8));
      }
      else
      {
         release_cursor ();
      }
#endif
      
      _tracker_value.stop (event);
   }
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FrameMusicTime::mouse_dragged (opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_value.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_value.track (event);
   }
}



/*
==============================================================================
Name: mouse_button_pressed_twice
==============================================================================
*/

void  FrameMusicTime::mouse_button_pressed_twice (opal::Event & event)
{
   event.block ();
}



/*
==============================================================================
Name: mouse_button_released_twice
==============================================================================
*/

void  FrameMusicTime::mouse_button_released_twice (opal::Event & event)
{
   _widget.req_edit (_field);
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameMusicTime::user_input_lost (opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_value.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_value.stop (event);
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

