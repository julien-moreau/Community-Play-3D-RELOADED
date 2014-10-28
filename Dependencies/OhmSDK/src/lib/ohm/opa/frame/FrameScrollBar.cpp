/*****************************************************************************

        FrameScrollBar.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69631

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

#include "ohm/opa/frame/FrameScrollBar.h"
#include "ohm/opa/WidgetScrollBar.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: FrameScrollBar
==============================================================================
*/

FrameScrollBar::FrameScrollBar (WidgetScrollBar & widget)
:  opal::Frame ("FrameScrollBar")
,  _widget (widget)
,  _dispatcher (*this)
,  _tracker (*this)
,  _content_length (0.f)
,  _visible_position (0.f)
,  _visible_length (0.f)
{
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameScrollBar>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameScrollBar>::ClickCount_ONE,
      &FrameScrollBar::mouse_button_pressed,
      &FrameScrollBar::mouse_button_released,
      &FrameScrollBar::mouse_dragged
   );
   
   _dispatcher._user_input.configure_lost (
      &FrameScrollBar::user_input_lost
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name: ~FrameScrollBar
==============================================================================
*/

FrameScrollBar::~FrameScrollBar ()
{
}



/*
==============================================================================
Name: mouse_target_lost
==============================================================================
*/

void  FrameScrollBar::mouse_target_lost (const opal::Event & event)
{
   _tracker.mouse_target_lost ();
}



/*
==============================================================================
Name: set_content_length
==============================================================================
*/

void  FrameScrollBar::set_content_length (float length)
{
   _content_length = length;
   
   update ();
}



/*
==============================================================================
Name: set_visible_position
==============================================================================
*/

void  FrameScrollBar::set_visible_position (float position)
{
   _visible_position = position;
   
   update ();
}



/*
==============================================================================
Name: set_visible_length
==============================================================================
*/

void  FrameScrollBar::set_visible_length (float length)
{
   _visible_length = length;
   
   update ();
}



/*
==============================================================================
Name: set_visible_length
==============================================================================
*/

void  FrameScrollBar::frame_bounds_changed ()
{
   update ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: mouse_button_pressed
==============================================================================
*/

void FrameScrollBar::mouse_button_pressed (opal::Event & event)
{
   _tracker.start (event);
}



/*
==============================================================================
Name: mouse_button_released
==============================================================================
*/

void FrameScrollBar::mouse_button_released (opal::Event & event)
{
   _tracker.stop (event);
}



/*
==============================================================================
Name: mouse_dragged
==============================================================================
*/

void FrameScrollBar::mouse_dragged (opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker.track (event);
   }
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameScrollBar::user_input_lost (opal::Event & event)
{
   bool tracking_flag = false;
   tracking_flag |= _tracker.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker.stop (event);
   }
}



/*
==============================================================================
Name: TrackerValue
==============================================================================
*/

FrameScrollBar::TrackerValue::TrackerValue (FrameScrollBar & frame)
:  opal::TrackerBase (frame)
,  _frame (frame)
{
}



/*
==============================================================================
Name: do_start
==============================================================================
*/

bool  FrameScrollBar::TrackerValue::do_start (opal::Event & event)
{
   return _frame.start (event);
}



/*
==============================================================================
Name: do_track
==============================================================================
*/

void  FrameScrollBar::TrackerValue::do_track (opal::Event & event)
{
   _frame.track (event);
}



/*
==============================================================================
Name: start
==============================================================================
*/

bool  FrameScrollBar::start (opal::Event & event)
{
   bool ret_val = true;
   
   opa::Point mouse_location = event._mouse._location;
   
   float location = _widget.is_horizontal () ? mouse_location._x : mouse_location._y;

   ret_val &= location >= _handle_position;
   ret_val &= location <= _handle_position + _handle_length;
   
   _click_position = _visible_position;

   return ret_val;
}



/*
==============================================================================
Name: track
==============================================================================
*/

void  FrameScrollBar::track (opal::Event & event)
{
   opa::Point mouse_click_location = event._mouse._click_location;
   opa::Point mouse_location = event._mouse._location;
   
   float location_delta
      = _widget.is_horizontal ()
      ? mouse_location._x - mouse_click_location._x
      : mouse_location._y - mouse_click_location._y;
   
   float length = _widget.is_horizontal () ? get_bounds ()._width : get_bounds ()._height;
   
   if (length - _handle_length > 0.f)
   {
      float mul = (_content_length - _visible_length) / (length - _handle_length);
      float visible_position = _click_position + location_delta * mul;
      
      _widget.req_set_position (visible_position);
   }
}



/*
==============================================================================
Name: update
==============================================================================
*/

void  FrameScrollBar::update ()
{
   bool active_flag = true;
   active_flag &= _visible_length < _content_length;
   active_flag &= _content_length > 0.f;
   active_flag &= _visible_length > 0.f;
   
   float length;

   if (active_flag)
   {
      length = _widget.is_horizontal () ? get_bounds ()._width : get_bounds ()._height;
      
      _handle_length = length * _visible_length / _content_length;
      
      lang::max_in_place (_handle_length, 16.f);


      if (_content_length - _visible_length == 0)
      {
         _handle_position = 0;
      }
      else
      {
         _handle_position
            = (length - _handle_length - 2)
            * _visible_position / (_content_length - _visible_length);
      }
      _handle_position += 1.f;
      
      active_flag &= 1.f < length - _handle_length - 2.f;
   }
   
   if (active_flag)
   {
      lang::limit_in_place (
         _handle_position,
         1.f,
         length - _handle_length - 2.f
      );
   }
   else
   {
      _handle_position = 0.f;
      _handle_length = 0.f;
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

