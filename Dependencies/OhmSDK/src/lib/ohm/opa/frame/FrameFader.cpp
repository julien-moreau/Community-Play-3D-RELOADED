/*****************************************************************************

        FrameFader.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 62138

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

#include "ohm/opa/frame/FrameFader.h"
#include "ohm/opa/WidgetFader.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: FrameFader
==============================================================================
*/

FrameFader::FrameFader (WidgetFader & widget)
:  opal::Frame ("FrameFader")
,  _widget (widget)
,  _dispatcher (*this)
,  _tracker (*this, widget)
{
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameFader>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameFader>::ClickCount_ONE,
      &FrameFader::mouse_button_pressed,
      &FrameFader::mouse_button_released,
      &FrameFader::mouse_dragged
   );
   
   _dispatcher._mouse.configure_mouse_button_pressed (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameFader>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameFader>::ClickCount_TWO,
      &FrameFader::mouse_button_pressed_twice
   );

   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameFader>::Button_LEFT,
      opal::EventDispatcherStandardMouse <FrameFader>::ClickCount_TWO,
      &FrameFader::mouse_button_released_twice
   );

   add (_dispatcher);
   
   set_no_clip ();
}



/*
==============================================================================
Name: ~FrameFader
==============================================================================
*/

FrameFader::~FrameFader ()
{
}



/*
==============================================================================
Name: set_horizontal
==============================================================================
*/

void FrameFader::set_horizontal (bool flag)
{
   _tracker._horizontal_flag = flag;
}



/*
==============================================================================
Name: mouse_target_lost
==============================================================================
*/

void FrameFader::mouse_target_lost (const opal::Event & event)
{
   _tracker.mouse_target_lost ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: mouse_button_pressed
==============================================================================
*/

void FrameFader::mouse_button_pressed (opal::Event & event)
{
   _tracker.start (event);
}



/*
==============================================================================
Name: mouse_button_released
==============================================================================
*/

void FrameFader::mouse_button_released (opal::Event & event)
{
   _tracker.stop (event);
}



/*
==============================================================================
Name: mouse_dragged
==============================================================================
*/

void FrameFader::mouse_dragged (opal::Event & event)
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
Name: mouse_button_pressed_twice
==============================================================================
*/

void  FrameFader::mouse_button_pressed_twice (opal::Event & event)
{
   if (is_in (event))
   {
      event.block ();
   }
}



/*
==============================================================================
Name: mouse_button_released_twice
==============================================================================
*/

void  FrameFader::mouse_button_released_twice (opal::Event & event)
{
   if (is_in (event))
   {
      event.block ();
      
      _widget.req_set_value (0.0, WidgetFader::Event::Type_RESET);
   }
}



/*
==============================================================================
Name: is_in
==============================================================================
*/

bool  FrameFader::is_in (opal::Event & event)
{
   opa::Rect rect (
      opa::Point::_zero, get_bounds ()
   );
   
   Margin margin;
   
   if (_tracker._horizontal_flag)
   {
      margin._left = 6;
      margin._right = 6;
   }
   else
   {
      margin._top = 6;
      margin._bottom = 6;
   }

   rect += margin;
   
   bool ret_val = rect.contains (event._mouse._location);
   
   return ret_val;
}



/*
==============================================================================
Name: TrackerValue
==============================================================================
*/

FrameFader::TrackerValue::TrackerValue (FrameFader & frame, WidgetFader & bundle)
:  opal::TrackerBase (frame)
,  _bundle (bundle)
,  _frame (frame)
,  _horizontal_flag (true)
{
}



/*
==============================================================================
Name: do_start
==============================================================================
*/

bool FrameFader::TrackerValue::do_start (opal::Event & event)
{
   if (_frame.is_in (event))
   {
      float req_value = norm_value_for_event (event);
      
      _bundle.req_set_value (req_value, WidgetFader::Event::Type_BEGIN);

      return true;
   }
   
   return false;
}



/*
==============================================================================
Name: do_track
==============================================================================
*/

void FrameFader::TrackerValue::do_track (opal::Event & event)
{
   const float req_value = norm_value_for_event (event);
   
   _bundle.req_set_value (req_value, WidgetFader::Event::Type_GESTURE);
}



/*
==============================================================================
Name: do_commit
==============================================================================
*/

void FrameFader::TrackerValue::do_commit (opal::Event & event)
{
   const float req_value = norm_value_for_event (event);
   
   _bundle.req_set_value (req_value, WidgetFader::Event::Type_COMMIT);
}



/*
==============================================================================
Name: do_revert
==============================================================================
*/

void FrameFader::TrackerValue::do_revert ()
{
   _bundle.req_set_value (0, WidgetFader::Event::Type_CANCEL);
}



/*
==============================================================================
Name: norm_value_for_event
==============================================================================
*/

float FrameFader::TrackerValue::norm_value_for_event (opal::Event & event) const
{
   float norm_value;
   
   if (_horizontal_flag)
   {
      const float width = _frame.get_bounds ()._width;
       
      norm_value = lang::limit (
         float (event._mouse._location._x),
         0.f, width
      );
      
      if (width != 0)
      {
         norm_value /= width;
      }
   }
   else
   {
      const float height = _frame.get_bounds ()._height;
       
      norm_value = lang::limit (
         float (event._mouse._location._y),
         0.f, height
      );

      if (height != 0)
      {
         norm_value /= height;
      }
   }

   return norm_value;
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

