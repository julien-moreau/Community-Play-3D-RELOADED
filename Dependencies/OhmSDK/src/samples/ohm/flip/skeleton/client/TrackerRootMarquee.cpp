/*****************************************************************************

        TrackerRootMarquee.cpp
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

#include "client/TrackerRootMarquee.h"

#include "client/Conduct.h"
#include "client/BundleRoot.h"
#include "client/FrameRoot.h"

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

TrackerRootMarquee::TrackerRootMarquee (BundleRoot & bundle, FrameRoot & frame)
:  _bundle (bundle)
,  _frame (frame)
,  _state (State_VOID)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerRootMarquee::~TrackerRootMarquee ()
{
   stop_revert ();
}



/*
==============================================================================
Name : is_triggered
==============================================================================
*/

bool  TrackerRootMarquee::is_triggered () const
{
   return (_state == State_TRIGGERED);
}



/*
==============================================================================
Name : is_tracking
==============================================================================
*/

bool  TrackerRootMarquee::is_tracking () const
{
   return (_state == State_TRACKING);
}



/*
==============================================================================
Name : trigger
==============================================================================
*/

void  TrackerRootMarquee::trigger ()
{
   assert (_state == State_VOID);
   
   _state = State_TRIGGERED;
   
   _frame.acquire_mouse_capture ();
}



/*
==============================================================================
Name : start
==============================================================================
*/

void  TrackerRootMarquee::start (ohm::opal::Event & event)
{
   assert (_state == State_TRIGGERED);

   _origin = event._mouse._click_location;
   
   _bundle.req_start_marquee (_origin);

   _state = State_TRACKING;
   
   event.block ();
}



/*
==============================================================================
Name : track
==============================================================================
*/

void  TrackerRootMarquee::track (ohm::opal::Event & event)
{
   using namespace ohm;
   
   if (_state == State_TRACKING)
   {
      opa::Point dpt = event._mouse._location - _origin;
      
      _bundle.req_change_marquee_size (opa::Size (dpt._x, dpt._y));
      
      event.block ();
   }
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  TrackerRootMarquee::stop (ohm::opal::Event & event)
{
   using namespace ohm;
   
   assert (
      (_state == State_VOID)
      || (_state == State_TRIGGERED)
      || (_state == State_TRACKING)
   );
   
   if (_state == State_TRACKING)
   {
      _state = State_STOPPING;
      
      // will call mouse_target_lost synchronously
      _frame.release_mouse_capture ();
      
      bool commit_flag = event._type != opal::Event::Type_USER_INPUT_LOST;
      
      if (commit_flag)
      {
         bool toggle_flag = event._key.has_shift_modifier ();
   
         // push child transform as we are going to fetch the matrix in
         // ConductWkspMarquee
         
         _bundle.req_end_marquee (toggle_flag);
         
         event.block ();
      }
      else
      {
         _bundle.req_change_marquee_size (opa::Size::_zero);
      }
   }
   else if (_state == State_TRIGGERED)
   {
      _state = State_STOPPING;
      
      _frame.release_mouse_capture ();
   }

   _state = State_VOID;
   
   Conduct * conduct_ptr = event.get <Conduct> ();
   
   if (conduct_ptr != 0)
   {
      conduct_ptr->_invalid_tool_flag = true;
      _frame.post_conduct_changed_event (*conduct_ptr);
   }
}



/*
==============================================================================
Name : stop_revert
==============================================================================
*/

void  TrackerRootMarquee::stop_revert ()
{
   using namespace ohm;
   
   assert (
      (_state == State_VOID)
      || (_state == State_TRIGGERED)
      || (_state == State_TRACKING)
   );
   
   if (_state == State_TRACKING)
   {
      _state = State_REVERTING;
      
      // will call mouse_target_lost synchronously
      _frame.release_mouse_capture ();
      
      _bundle.req_change_marquee_size (opa::Size::_zero);
   }
   else if (_state == State_TRIGGERED)
   {
      _state = State_REVERTING;
      
      _frame.release_mouse_capture ();
   }
   
   _state = State_VOID;
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  TrackerRootMarquee::mouse_target_lost ()
{
   using namespace ohm;
   
   // we might be call synchronously from a release_mouse_capture,
   // so ignore if the state is State_STOPPING or State_REVERTING
   
   if (_state == State_TRACKING)
   {
      _bundle.req_change_marquee_size (opa::Size::_zero);
   }
   
   _state = State_VOID;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
