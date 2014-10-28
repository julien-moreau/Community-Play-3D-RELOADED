/*****************************************************************************

        FrameRoot.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71268

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

#include "client/FrameRoot.h"

#include "client/BundleRoot.h"
#include "client/Conduct.h"
#include "client/Constants.h"
#include "client/Cursor.h"

#include "model/Root.h"

#include "ohm/opal/Frame.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameRoot::FrameRoot (BundleRoot & bundle)
:  ohm::opal::Frame ("FrameRoot")
,  _bundle (bundle)
,  _dispatcher (*this)

,  _tracker_marquee (bundle, *this)
,  _tracker_hand (bundle, *this)
{
   using namespace ohm;
   
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameRoot>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameRoot>::ClickCount_ONE,
      &FrameRoot::mouse_button_pressed,
      &FrameRoot::mouse_button_released,
      &FrameRoot::mouse_dragged
   );

   _dispatcher._mouse.configure_cursor (
      &FrameRoot::mouse_cursor
   );

   _dispatcher._mouse.configure_wheel (
      &FrameRoot::mouse_wheel_moved
   );
   
   _dispatcher._keyboard.configure_key (
      opal::Event::Pass_CRUMB,
      &FrameRoot::crumb_key_pressed,
      &FrameRoot::crumb_key_pressed,   // repeated
      &FrameRoot::crumb_key_released
   );
   
   _dispatcher._user_input.configure_lost (
      &FrameRoot::user_input_lost
   );

   _dispatcher._conduct.configure (
      opal::Event::Pass_CRUMB,
      &FrameRoot::conduct_changed
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameRoot::~FrameRoot ()
{
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  FrameRoot::mouse_target_lost (const ohm::opal::Event & event)
{
   _tracker_marquee.mouse_target_lost ();
   _tracker_hand.mouse_target_lost ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FrameRoot::mouse_button_pressed (ohm::opal::Event & event)
{
   const Conduct * conduct_ptr = event.get <Conduct> ();
   assert (conduct_ptr != 0);
   
   model::Tool tool = conduct_ptr->_tool;
      
   if (
       (tool == model::Tool_ARROW)
       || (tool == model::Tool_ERASER)
       )
   {
      propagate_children (event);
      
      if (!event.is_blocked ())
      {  
         _tracker_marquee.trigger ();
      
         event.block ();
      }
   }
   else if (conduct_ptr->_tool == model::Tool_HAND)
   {
      propagate_children (event);
      
      if (!event.is_blocked ())
      {  
         _tracker_hand.start (event);
      }
   }
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  FrameRoot::mouse_button_released (ohm::opal::Event & event)
{  
   bool tracking_flag = false;
   tracking_flag |= _tracker_marquee.is_tracking ();
   tracking_flag |= _tracker_hand.is_tracking ();
   
   if (!tracking_flag)
   {
      propagate_children (event);
      
      if (!event.is_blocked ())
      {
         _bundle.req_clear_selection ();
         
         event.block ();
      }
   }

   _tracker_marquee.stop (event);
   _tracker_hand.stop (event);
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FrameRoot::mouse_dragged (ohm::opal::Event & event)
{
   Conduct * conduct_ptr = event.get <Conduct> ();
   assert (conduct_ptr);

   bool tracking_flag = false;
   tracking_flag |= _tracker_marquee.is_tracking ();
   tracking_flag |= _tracker_hand.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_marquee.track (event);
      _tracker_hand.track (event);
   }
   else
   {
      propagate_children (event);
      
      if (!event.is_blocked ())
      {
         model::Tool tool = conduct_ptr->_tool;
         
         if (
             (tool == model::Tool_ARROW)
             || (tool == model::Tool_ERASER)
             )
         {
            if (_tracker_marquee.is_triggered ())  
            {
               _tracker_marquee.start (event);
            }
         }
      }
   }
}



/*
==============================================================================
Name : mouse_cursor
==============================================================================
*/

void  FrameRoot::mouse_cursor (ohm::opal::Event & event)
{
   propagate_children (event);
   
   if (event.is_blocked ())
   {
      return;
   }
   
   const Conduct * conduct_ptr = event.get <Conduct> ();
   assert (conduct_ptr != 0);
   
   if (conduct_ptr->_tool == model::Tool_PEN_TAXI)
   {
      Cursor::use ().pen ().set ();
   }
   else if (conduct_ptr->_tool == model::Tool_PEN_PEDESTRIAN)
   {
      Cursor::use ().pen ().set ();
   }
   else if (conduct_ptr->_tool == model::Tool_ERASER)
   {
      Cursor::use ().eraser ().set ();
   }
   else if (conduct_ptr->_tool == model::Tool_HAND)
   {
      bool pressed_flag
         = (event._mouse._pressed_flag)
         && (event._mouse._button_nbr == 1);
      
      _tracker_hand.update_cursor (pressed_flag);
   }
   else
   {
      Cursor::use ().arrow ().set ();
   }
   
   event.block ();
}



/*
==============================================================================
Name : mouse_wheel_moved
==============================================================================
*/

void  FrameRoot::mouse_wheel_moved (ohm::opal::Event & event)
{
   using namespace ohm;
   
   propagate_children (event);
   
   if (!event.is_blocked ())
   {
      opa::Point delta = event._mouse._wheel_delta;
      opa::Point location = event._mouse._location;
      
      delta._x = delta._x * 28.f;
      delta._y = delta._y * 28.f;
      
      _bundle.req_focus_delta (delta);
      
      event.block ();
   }
}



/*
==============================================================================
Name : crumb_key_pressed
==============================================================================
*/

void  FrameRoot::crumb_key_pressed (ohm::opal::Event & event)
{
   using namespace ohm;
   
   propagate_children (event);
   
   if (!event.is_blocked ())
   {
      Conduct * conduct_ptr = event.get <Conduct> ();
      assert (conduct_ptr);
      
      bool shift_flag = event._key.has_shift_modifier ();
      bool alt_flag = event._key.has_alt_modifier ();
      
      if ((event._key == opal::Key (opal::KeyCode_BACKSPACE)) || (event._key == opal::Key (opal::KeyCode_DEL)))
      {
         _bundle.req_delete_selection ();
         
         event.block ();
      }
      else if (event._key.get_key_code () == opal::KeyCode_LEFT_ARROW)
      {
         _bundle.req_move_selection (opa::Point (-1, 0), shift_flag, alt_flag);
         
         event.block ();
      }
      else if (event._key.get_key_code () == opal::KeyCode_RIGHT_ARROW)
      {
         _bundle.req_move_selection (opa::Point (1, 0), shift_flag, alt_flag);
         
         event.block ();
      }
      else if (event._key.get_key_code () == opal::KeyCode_UP_ARROW)
      {
         _bundle.req_move_selection (opa::Point (0, -1), shift_flag, alt_flag);
         
         event.block ();
      }
      else if (event._key.get_key_code () == opal::KeyCode_DOWN_ARROW)
      {
         _bundle.req_move_selection (opa::Point (0, 1), shift_flag, alt_flag);
         
         event.block ();
      }
   }
}



/*
==============================================================================
Name : crumb_key_released
==============================================================================
*/

void  FrameRoot::crumb_key_released (ohm::opal::Event & event)
{
   propagate_children (event);
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameRoot::user_input_lost (ohm::opal::Event & event)
{
   _tracker_marquee.stop (event);
   _tracker_hand.stop (event);
}



/*
==============================================================================
Name : conduct_changed
==============================================================================
*/

void  FrameRoot::conduct_changed (ohm::opal::Event & event)
{
   propagate_children (event);
   
   {
      ConductMarquee * conduct_marquee_ptr = event.get <ConductMarquee> ();
      assert (conduct_marquee_ptr != 0);
      
      if (conduct_marquee_ptr->is_collecting ())
      {
         _bundle.req_select_marquee ();
         
         conduct_marquee_ptr->end_collecting ();
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
