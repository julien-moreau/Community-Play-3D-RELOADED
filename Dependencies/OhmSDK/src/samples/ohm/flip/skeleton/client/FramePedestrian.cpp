/*****************************************************************************

        FramePedestrian.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71391

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

#include "client/FramePedestrian.h"

#include "client/BundlePedestrian.h"
#include "client/Conduct.h"
#include "client/ConductMarquee.h"
#include "client/Constants.h"

#include "model/Pedestrian.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FramePedestrian::FramePedestrian (BundlePedestrian & bundle)
:  ohm::opal::Frame ("FramePedestrian")
,  _bundle (bundle)
,  _dispatcher (*this)

,  _tracker_position (bundle, *this)
{
   using namespace ohm;
   
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FramePedestrian>::Button_LEFT,
      &FramePedestrian::mouse_button_pressed,
      &FramePedestrian::mouse_button_released,
      &FramePedestrian::mouse_dragged
   );
   
   _dispatcher._mouse.configure_mouse_moved (
      &FramePedestrian::mouse_moved
   );
   
   _dispatcher._keyboard.configure_key (
      opal::Event::Pass_FOCUS,
      &FramePedestrian::focus_key_changed,
      &FramePedestrian::focus_key_changed
   );
   
   _dispatcher._user_input.configure_lost (
      &FramePedestrian::user_input_lost
   );
   
   _dispatcher._conduct.configure (
      opal::Event::Pass_CRUMB,
      &FramePedestrian::conduct_changed
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FramePedestrian::~FramePedestrian ()
{
}



/*
==============================================================================
Name : mouse_entered
==============================================================================
*/

void  FramePedestrian::mouse_entered (ohm::opal::Event & event)
{
   _bundle.req_highlight (true);
}



/*
==============================================================================
Name : mouse_exited
==============================================================================
*/

void  FramePedestrian::mouse_exited (ohm::opal::Event & event)
{
   _bundle.req_highlight (false);
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  FramePedestrian::mouse_target_lost (const ohm::opal::Event & event)
{
   _tracker_position.mouse_target_lost ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FramePedestrian::mouse_button_pressed (ohm::opal::Event & event)
{
   const Conduct * conduct_ptr = event.get <Conduct> ();
   assert (conduct_ptr != 0);

   if (
      (conduct_ptr->_tool == model::Tool_ARROW)
      || (conduct_ptr->_tool == model::Tool_PEN_TAXI)
      || (conduct_ptr->_tool == model::Tool_PEN_PEDESTRIAN)
      )
   {
      bool toggle_flag = event._key.has_shift_modifier ();
      
      _bundle.req_select (toggle_flag);
      
      _tracker_position.start (event);
   }
   else if (conduct_ptr->_tool == model::Tool_ERASER)
   {
      bool toggle_flag = event._key.has_shift_modifier ();
      
      _bundle.req_select (toggle_flag);
   }
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  FramePedestrian::mouse_button_released (ohm::opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_position.is_tracking ();
   
   if (!tracking_flag)
   {
      const Conduct * conduct_ptr = event.get <Conduct> ();
      assert (conduct_ptr != 0);
      
      if (conduct_ptr->_tool == model::Tool_ERASER)
      {
         _bundle.req_delete ();
         
         event.block ();
      }
   }

   _tracker_position.stop (event);
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FramePedestrian::mouse_dragged (ohm::opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_position.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_position.track (event);
   }
}



/*
==============================================================================
Name : mouse_moved
==============================================================================
*/

void  FramePedestrian::mouse_moved (ohm::opal::Event & event)
{
   // nothing
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FramePedestrian::user_input_lost (ohm::opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_position.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_position.stop (event);
   }
}



/*
==============================================================================
Name : conduct_changed
==============================================================================
*/

void  FramePedestrian::conduct_changed (ohm::opal::Event & event)
{
   using namespace ohm;
   
   ConductMarquee * conduct_marquee_ptr = event.get <ConductMarquee> ();
   assert (conduct_marquee_ptr != 0);
   
   if (conduct_marquee_ptr->is_collecting ())
   {
      opa::Rect rect;
      rect._size._width  = Constants::_metric_pedestrian_width;
      rect._size._height = Constants::_metric_pedestrian_height;
      
      bool add_flag = conduct_marquee_ptr->is_intersecting (rect);
      
      if (add_flag)
      {
         conduct_marquee_ptr->add (_bundle.use_pedestrian ());
      }
   }
}



/*
==============================================================================
Name: focus_key_changed
==============================================================================
*/

void FramePedestrian::focus_key_changed (ohm::opal::Event & event)
{
   _tracker_position.key_changed (event);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
