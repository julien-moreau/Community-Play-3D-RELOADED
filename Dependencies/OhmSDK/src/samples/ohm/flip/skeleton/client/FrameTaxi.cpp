/*****************************************************************************

        FrameTaxi.cpp
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

#include "client/FrameTaxi.h"

#include "client/BundleTaxi.h"
#include "client/Cursor.h"
#include "client/Conduct.h"
#include "client/ConductMarquee.h"
#include "client/Constants.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameTaxi::FrameTaxi (BundleTaxi & bundle)
:  ohm::opal::Frame ("FrameTaxi")
,  _bundle (bundle)
,  _dispatcher (*this)

,  _tracker_position (bundle, *this)
,  _tracker_width (bundle, *this)
{
   using namespace ohm;
   
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameTaxi>::Button_LEFT,
      &FrameTaxi::mouse_button_pressed,
      &FrameTaxi::mouse_button_released,
      &FrameTaxi::mouse_dragged
   );
   
   _dispatcher._mouse.configure_mouse_moved (
      &FrameTaxi::mouse_moved
   );
   
   _dispatcher._mouse.configure_cursor (
      &FrameTaxi::mouse_cursor
   );
   
   _dispatcher._keyboard.configure_key (
      opal::Event::Pass_FOCUS,
      &FrameTaxi::focus_key_changed,
      &FrameTaxi::focus_key_changed
   );
   
   _dispatcher._user_input.configure_lost (
      &FrameTaxi::user_input_lost
   );
   
   _dispatcher._conduct.configure (
      opal::Event::Pass_CRUMB,
      &FrameTaxi::conduct_changed
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameTaxi::~FrameTaxi ()
{
}



/*
==============================================================================
Name : mouse_entered
==============================================================================
*/

void  FrameTaxi::mouse_entered (ohm::opal::Event & event)
{
   _bundle.req_highlight (true);
}



/*
==============================================================================
Name : mouse_exited
==============================================================================
*/

void  FrameTaxi::mouse_exited (ohm::opal::Event & event)
{
   _bundle.req_highlight (false);
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  FrameTaxi::mouse_target_lost (const ohm::opal::Event & event)
{
   _tracker_position.mouse_target_lost ();
   _tracker_width.mouse_target_lost ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FrameTaxi::mouse_button_pressed (ohm::opal::Event & event)
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
      
      MouseZone mouse_zone = get_mouse_zone (event);
      
      switch (mouse_zone)
      {
      case MouseZone_POSITION:
         _tracker_position.start (event);
         break;

      case MouseZone_WIDTH:
         _tracker_width.start (event);
         break;
      }
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

void  FrameTaxi::mouse_button_released (ohm::opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_position.is_tracking ();
   tracking_flag |= _tracker_width.is_tracking ();
   
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
   _tracker_width.stop (event);
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FrameTaxi::mouse_dragged (ohm::opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_position.is_tracking ();
   tracking_flag |= _tracker_width.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_position.track (event);
      _tracker_width.track (event);
   }
}



/*
==============================================================================
Name : mouse_moved
==============================================================================
*/

void  FrameTaxi::mouse_moved (ohm::opal::Event & event)
{
   // nothing
}



/*
==============================================================================
Name : mouse_cursor
==============================================================================
*/

void  FrameTaxi::mouse_cursor (ohm::opal::Event & event)
{
   MouseZone mouse_zone = get_mouse_zone (event);
   
   switch (mouse_zone)
   {
   case MouseZone_POSITION:
      Cursor::use ().arrow ().set ();
      break;

   case MouseZone_WIDTH:
      Cursor::use ().resize ().right ().set ();
      break;
   }
   
   event.block ();
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameTaxi::user_input_lost (ohm::opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker_position.is_tracking ();
   tracking_flag |= _tracker_width.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker_position.stop (event);
      _tracker_width.stop (event);
   }
}



/*
==============================================================================
Name : conduct_changed
==============================================================================
*/

void  FrameTaxi::conduct_changed (ohm::opal::Event & event)
{
   using namespace ohm;
   
   ConductMarquee * conduct_marquee_ptr = event.get <ConductMarquee> ();
   assert (conduct_marquee_ptr != 0);
   
   if (conduct_marquee_ptr->is_collecting ())
   {
      opa::Rect rect;
      rect._size._width  = float (_bundle.use_taxi ().get_width ());
      rect._size._height = Constants::_metric_taxi_height;
      
      bool add_flag = conduct_marquee_ptr->is_intersecting (rect);
      
      if (add_flag)
      {
         conduct_marquee_ptr->add (_bundle.use_taxi ());
      }
   }
}



/*
==============================================================================
Name: focus_key_changed
==============================================================================
*/

void FrameTaxi::focus_key_changed (ohm::opal::Event & event)
{
   _tracker_position.key_changed (event);
   _tracker_width.key_changed (event);
}



/*
==============================================================================
Name : get_mouse_zone
==============================================================================
*/

FrameTaxi::MouseZone FrameTaxi::get_mouse_zone (ohm::opal::Event & event)
{
   using namespace ohm;
   
   bool in_flag = false;
   
   float width = float (_bundle.use_taxi ().get_width ());
   
   opa::Size dsz (width, 0.f);
   event.apply_inverse_transform (dsz);
   
   float right = float (_bundle.use_taxi ().get_width ());
   float x = event._mouse._location._x;
      
   float dist = lang::abs (right - x);
   opa::Size sz (dist, 0.f);
   event.apply_inverse_transform (sz);
   
   in_flag = sz.norm_1 () <= 6.f;
   
   // if the notes is beginning to be too small, deactivate duration in the inner note
   
   if (dsz.norm_1 () < 12.f)
   {
      in_flag &= x - right > 0;
   }
   
   return in_flag ? MouseZone_WIDTH : MouseZone_POSITION;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
