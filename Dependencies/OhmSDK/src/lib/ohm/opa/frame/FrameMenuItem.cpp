/*****************************************************************************

        FrameMenuItem.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50638

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

#include "ohm/opa/frame/FrameMenuItem.h"
#include "ohm/opa/WidgetMenuItem.h"


#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: FrameMenuItem
==============================================================================
*/

FrameMenuItem::FrameMenuItem (WidgetMenuItem & widget)
:  opal::Frame ("FrameMenuItem")
,  _widget (widget)
,  _dispatcher (*this)
,  _mouse_moved_flag (false)
{  
   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameMenuItem>::Button_LEFT, 
      opal::EventDispatcherStandardMouse <FrameMenuItem>::ClickCount_ONE, 
      &FrameMenuItem::mouse_button_released
   );

   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameMenuItem>::Button_RIGHT, 
      opal::EventDispatcherStandardMouse <FrameMenuItem>::ClickCount_ONE, 
      &FrameMenuItem::mouse_button_released
   );

   _dispatcher._mouse.configure_mouse_dragged (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameMenuItem>::Button_LEFT, 
      opal::EventDispatcherStandardMouse <FrameMenuItem>::ClickCount_ONE, 
      &FrameMenuItem::mouse_moved
   );

   _dispatcher._mouse.configure_mouse_dragged (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameMenuItem>::Button_RIGHT, 
      opal::EventDispatcherStandardMouse <FrameMenuItem>::ClickCount_ONE, 
      &FrameMenuItem::mouse_moved
   );

   _dispatcher._mouse.configure_mouse_moved (
      opal::Event::Pass_SPATIAL,
      &FrameMenuItem::mouse_moved
   );
   
   _dispatcher._user_input.configure_lost (&FrameMenuItem::user_input_lost);
   
   add (_dispatcher);
}



/*
==============================================================================
Name: ~FrameMenuItem
==============================================================================
*/

FrameMenuItem::~FrameMenuItem ()
{
}



/*
==============================================================================
Name: mouse_button_released
==============================================================================
*/

void FrameMenuItem::mouse_button_released (opal::Event & event)
{
   assert (&event != 0);
   
   propagate_children (event);
   
   if (!event.is_blocked ())
   {
      // no clip : we have to check the bounds
      Rect  rect (Point::_zero, get_bounds ());

      if (
         rect.contains (event._mouse._location)
         && _widget.can_select ()
         && _mouse_moved_flag
         )
      {
         event.block ();
         
         _widget.req_select (event);
      }
   }
}



/*
==============================================================================
Name: mouse_moved
==============================================================================
*/

void FrameMenuItem::mouse_moved (opal::Event & event)
{
   assert (&event != 0);
   
   _mouse_moved_flag = true;
}



/*
==============================================================================
Name: user_input_lost
==============================================================================
*/

void FrameMenuItem::user_input_lost (opal::Event & event)
{
   assert (&event != 0);
}



/*
==============================================================================
Name: mouse_entered
==============================================================================
*/

void FrameMenuItem::mouse_entered (opal::Event & event)
{
   _widget.req_entered ();
}



/*
==============================================================================
Name: mouse_exited
==============================================================================
*/

void FrameMenuItem::mouse_exited (opal::Event & event)
{
   _widget.req_exited ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


   
}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

