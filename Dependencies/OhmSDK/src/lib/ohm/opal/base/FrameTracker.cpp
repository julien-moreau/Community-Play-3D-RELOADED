/*****************************************************************************

        FrameTracker.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58326

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

#include "ohm/opal/base/FrameTracker.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Event::ctor
==============================================================================
*/

FrameTracker::Event::Event (FrameTracker & frame)
:  _frame (frame)
,  _event_ptr (0)
,  _type (Type_NONE)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

FrameTracker::FrameTracker ()
:  opal::Frame ("FrameTracker")
,  _dispatcher (*this)
,  _event (*this)
,  _tracker (*this)
{
   _dispatcher._mouse.configure_gesture (
      opal::EventDispatcherStandardMouse <FrameTracker>::Button_LEFT,
      &FrameTracker::mouse_button_pressed,
      &FrameTracker::mouse_button_released,
      &FrameTracker::mouse_dragged
   );

   _dispatcher._mouse.configure_mouse_moved (
      &FrameTracker::mouse_dragged
   );

   _dispatcher._user_input.configure_lost (
      &FrameTracker::user_input_lost
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameTracker::~FrameTracker ()
{
}

   
   
/*
==============================================================================
Name : mouse_button_pressed
==============================================================================
*/

void  FrameTracker::mouse_button_pressed (opal::Event & event)
{
   _tracker.start (event);
}



/*
==============================================================================
Name : mouse_button_released
==============================================================================
*/

void  FrameTracker::mouse_button_released (opal::Event & event)
{
   _tracker.stop (event);
}



/*
==============================================================================
Name : mouse_dragged
==============================================================================
*/

void  FrameTracker::mouse_dragged (opal::Event & event)
{
   bool tracking_flag = _tracker.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker.track (event);
   }
}



/*
==============================================================================
Name: key_pressed
==============================================================================
*/

void  FrameTracker::key_pressed (opal::Event & event)
{
   bool tracking_flag = _tracker.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker.track (event); 
   }
}



/*
==============================================================================
Name: key_released
==============================================================================
*/

void  FrameTracker::key_released (opal::Event & event)
{
   bool tracking_flag = _tracker.is_tracking ();
   
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

void  FrameTracker::user_input_lost (opal::Event & event)
{
   bool tracking_flag = _tracker.is_tracking ();
   
   if (tracking_flag)
   {
      _tracker.stop (event);  
   }
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  FrameTracker::mouse_target_lost (const opal::Event & event)
{
   _tracker.mouse_target_lost ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_start
==============================================================================
*/

bool  FrameTracker::Tracker::do_start (opal::Event & event)
{
   assert (_parent._event._type == FrameTracker::Event::Type_NONE);
   
   _parent._event._event_ptr = &event;
   _parent._event._type = FrameTracker::Event::Type_START;
   _parent._event._ok_flag = true;
   
   _parent._event._start_location = event._mouse._location;
   _parent._event._previous_location = event._mouse._location;
   _parent._event._location = event._mouse._location;
   
   _parent._delegate.param_ref_signal (_parent._event);

   _parent._event._type = FrameTracker::Event::Type_NONE;
   
   return _parent._event._ok_flag;
}



/*
==============================================================================
Name : do_track
==============================================================================
*/

void  FrameTracker::Tracker::do_track (opal::Event & event)
{
   assert (_parent._event._type == FrameTracker::Event::Type_NONE);
   
   _parent._event._event_ptr = &event;
   _parent._event._type = FrameTracker::Event::Type_TRACK;

   _parent._event._previous_location = _parent._event._location;
   _parent._event._location = event._mouse._location;
   
   _parent._delegate.param_ref_signal (_parent._event);

   _parent._event._type = FrameTracker::Event::Type_NONE;
}



/*
==============================================================================
Name : do_commit
==============================================================================
*/

void  FrameTracker::Tracker::do_commit (opal::Event & event)
{
   assert (_parent._event._type == FrameTracker::Event::Type_NONE);
   
   _parent._event._event_ptr = &event;
   _parent._event._type = FrameTracker::Event::Type_COMMIT;
   
   _parent._delegate.param_ref_signal (_parent._event);

   _parent._event._type = FrameTracker::Event::Type_NONE;
}



/*
==============================================================================
Name : do_revert
==============================================================================
*/

void  FrameTracker::Tracker::do_revert ()
{
   assert (_parent._event._type == FrameTracker::Event::Type_NONE);
   
   _parent._event._event_ptr = 0;
   _parent._event._type = FrameTracker::Event::Type_REVERT;
   
   _parent._delegate.param_ref_signal (_parent._event);

   _parent._event._type = FrameTracker::Event::Type_NONE;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
