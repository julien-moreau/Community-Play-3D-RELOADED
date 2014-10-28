/*****************************************************************************

        TrackerBase.cpp
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

#include "ohm/opal/base/TrackerBase.h"
#include "ohm/opal/Event.h"
#include "ohm/opal/Frame.h"

#include <cassert>



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

TrackerBase::TrackerBase (Frame & frame)
:  _frame (frame)
,  _state (State_VOID)
,  _anti_bad_move_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerBase::~TrackerBase ()
{
   assert (_state == State_VOID);
}



/*
==============================================================================
Name : enable_anti_bad_move
==============================================================================
*/

void  TrackerBase::enable_anti_bad_move (bool enable_flag)
{
   _anti_bad_move_flag = enable_flag;
}



/*
==============================================================================
Name : is_tracking
==============================================================================
*/

bool  TrackerBase::is_tracking () const
{
   return ((_state == State_TRACKING) || (_state == State_BEGIN_TRACKING));
}



/*
==============================================================================
Name : start
==============================================================================
*/

void  TrackerBase::start (Event & event)
{
   assert (event._type == Event::Type_MOUSE_BUTTON_PRESSED);
   assert (_state == State_VOID);
   
   bool tracking_flag = do_start (event);
   
   if (tracking_flag)
   {
      _state = State_BEGIN_TRACKING;
      _start_time = event._timer._time;
      
      _frame.acquire_mouse_capture ();
      
      event.block ();
   }
}



/*
==============================================================================
Name : track
==============================================================================
*/

void  TrackerBase::track (Event & event)
{
   assert (
      (_state == State_VOID)
      || (_state == State_BEGIN_TRACKING)
      || (_state == State_TRACKING)
   );
   
   if (_state == State_BEGIN_TRACKING)
   {
      if (_anti_bad_move_flag)
      {
         opa::Size delta_pt (
            event._mouse._view_relative_location._x - event._mouse._view_relative_click_location._x,
            event._mouse._view_relative_location._y - event._mouse._view_relative_click_location._y
         );
         
         double delta_time = event._timer._time - _start_time;
         
         if ((delta_pt.norm_1 () > 3) || (delta_time >= 1.0))
         {
            _state = State_TRACKING;
         }
      }
      else
      {
         _state = State_TRACKING;
      }
   }
   
   if (_state == State_TRACKING)
   {
      do_track (event);
      
      event.block ();
   }
}



/*
==============================================================================
Name : key_changed
==============================================================================
*/

void  TrackerBase::key_changed (Event & event)
{
   assert (
      (_state == State_VOID)
      || (_state == State_BEGIN_TRACKING)
      || (_state == State_TRACKING)
   );
   
   if (_state == State_BEGIN_TRACKING)
   {
      _state = State_TRACKING;
   }
   
   if (_state == State_TRACKING)
   {
      do_key_changed (event);

      do_track (event);
      
      // do not block event
   }
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  TrackerBase::stop (Event & event)
{
   assert (
      (_state == State_VOID)
      || (_state == State_BEGIN_TRACKING)
      || (_state == State_TRACKING)
   );
   
   if ((_state == State_TRACKING) || (_state == State_BEGIN_TRACKING))
   {
      _state = State_STOPPING;
      
      // will call mouse_target_lost synchronously
      _frame.release_mouse_capture ();
      
      bool commit_flag = event._type != Event::Type_USER_INPUT_LOST;
      
      if (commit_flag)
      {
         do_commit (event);
         
         event.block ();
      }
      else
      {
         do_revert ();
      }
      
      _frame.post_update_cursor ();
      
      _state = State_VOID;
   }
}



/*
==============================================================================
Name : stop_revert
==============================================================================
*/

void  TrackerBase::stop_revert ()
{
   assert (
      (_state == State_VOID)
      || (_state == State_BEGIN_TRACKING)
      || (_state == State_TRACKING)
   );
   
   if ((_state == State_TRACKING) || (_state == State_BEGIN_TRACKING))
   {
      _state = State_REVERTING;
      
      // will call mouse_target_lost synchronously
      _frame.release_mouse_capture ();
      
      do_revert ();
      
      _frame.post_update_cursor ();
      
      _state = State_VOID;
   }
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  TrackerBase::mouse_target_lost ()
{
   // we might be call synchronously from a release_mouse_capture,
   // so ignore if the state is State_STOPPING or State_REVERTING
   
   if ((_state == State_TRACKING) || (_state == State_BEGIN_TRACKING))
   {
      do_revert ();
      
      _frame.post_update_cursor ();
      
      _state = State_VOID;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
