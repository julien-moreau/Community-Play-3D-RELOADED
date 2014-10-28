/*****************************************************************************

        Clock.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58088

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

#include "ohm/opal/private/Clock.h"
#include "ohm/opal/private/ClockListener.h"
#include "ohm/lang/fnc.h"

#include <cassert>
#include <cmath>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Clock::~Clock ()
{
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  Clock::add (ClockListener & listener, int stage)
{
   assert (&listener != 0);
   
   if (!_notifying_flag)
   {
      if (_inited_flag)
      {
         listener.init (_timestamp);
      }
      
      ClockListenerPtrSet & clock_listener_ptr_set
         = _stage_clock_listener_ptr_set_map [stage];
      
      clock_listener_ptr_set.insert (&listener);
   }
   else
   {
      StageClockListenerPtrPair sclpp (stage, &listener);
      
      _latent_insertion_clock_listener_ptr_list.push_back (sclpp);
   }
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  Clock::remove (ClockListener & listener)
{
   assert (&listener != 0);
   
   if (!_notifying_flag)
   {
      StageClockListenerPtrSetMap::iterator it
         = _stage_clock_listener_ptr_set_map.begin ();
      const StageClockListenerPtrSetMap::iterator it_end
         = _stage_clock_listener_ptr_set_map.end ();
      
      bool found_flag = false;
      
      for (; it != it_end ; ++it)
      {
         ClockListenerPtrSet & clock_listener_ptr_set = it->second;
         
         ClockListenerPtrSet::iterator sit = clock_listener_ptr_set.find (&listener);
         
         if (sit != clock_listener_ptr_set.end ())
         {
            clock_listener_ptr_set.erase (sit);
            found_flag = true;
            break;
         }
      }
      
      assert (found_flag);
   }
   else
   {
      _latent_removal_clock_listener_ptr_list.push_back (&listener);
   }
}


   
/*
==============================================================================
Name : init
==============================================================================
*/

void  Clock::init (double timestamp)
{
   //assert (!_inited_flag);
   
   _timestamp = timestamp;
   _inited_flag = true;
   
   init_listeners ();
}


   
/*
==============================================================================
Name : tick
==============================================================================
*/

void  Clock::tick (double timestamp)
{
   assert (_inited_flag);
   
   process_actions ();
   
   _timestamp = timestamp;
   
   notify_listeners ();
   
   process_actions ();
}


   
/*
==============================================================================
Name : get_time
==============================================================================
*/

double   Clock::get_time ()
{
   return _timestamp;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Clock::Clock ()
:  _stage_clock_listener_ptr_set_map ()
,  _inited_flag (false)
,  _timestamp (0.0)
,  _notifying_flag (false)
,  _latent_insertion_clock_listener_ptr_list ()
,  _latent_removal_clock_listener_ptr_list ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init_listeners
==============================================================================
*/

void  Clock::init_listeners ()
{
   _notifying_flag = true;
   
   StageClockListenerPtrSetMap::iterator it = _stage_clock_listener_ptr_set_map.begin ();
   const StageClockListenerPtrSetMap::iterator it_end = _stage_clock_listener_ptr_set_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ClockListenerPtrSet & clock_listener_ptr_set = it->second;
      
      init_listeners_stage (clock_listener_ptr_set);
   }
   
   _notifying_flag = false;
}



/*
==============================================================================
Name : init_listeners_stage
==============================================================================
*/

void  Clock::init_listeners_stage (ClockListenerPtrSet & clock_listener_ptr_set)
{
   ClockListenerPtrSet::iterator it = clock_listener_ptr_set.begin ();
   const ClockListenerPtrSet::iterator it_end = clock_listener_ptr_set.end ();
   
   for (; it != it_end ; ++it)
   {
      ClockListener * clock_listener_ptr = *it;
      
      if (!is_in_latent_removal_state (clock_listener_ptr))
      {
         clock_listener_ptr->init (get_time ());
      }
   }
}



/*
==============================================================================
Name : notify_listeners
==============================================================================
*/

void  Clock::notify_listeners ()
{
   _notifying_flag = true;
   
   StageClockListenerPtrSetMap::iterator it = _stage_clock_listener_ptr_set_map.begin ();
   const StageClockListenerPtrSetMap::iterator it_end = _stage_clock_listener_ptr_set_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ClockListenerPtrSet & clock_listener_ptr_set = it->second;
      
      notify_listeners_stage (clock_listener_ptr_set);
   }
   
   _notifying_flag = false;
}



/*
==============================================================================
Name : notify_listeners_stage
==============================================================================
*/

void  Clock::notify_listeners_stage (ClockListenerPtrSet & clock_listener_ptr_set)
{
   ClockListenerPtrSet::iterator it = clock_listener_ptr_set.begin ();
   const ClockListenerPtrSet::iterator it_end = clock_listener_ptr_set.end ();
   
   for (; it != it_end ; ++it)
   {
      ClockListener * clock_listener_ptr = *it;
      
      if (!is_in_latent_removal_state (clock_listener_ptr))
      {
         double timestamp = get_time ();
         
         clock_listener_ptr->tick (timestamp);
      }
   }
}



/*
==============================================================================
Name : process_actions
==============================================================================
*/

void  Clock::process_actions ()
{
   assert (!_notifying_flag);
   
   if (!_latent_insertion_clock_listener_ptr_list.empty ())
   {
      StageClockListenerPtrPairList::iterator it
         = _latent_insertion_clock_listener_ptr_list.begin ();

      const StageClockListenerPtrPairList::iterator it_end
         = _latent_insertion_clock_listener_ptr_list.end ();
      
      for (; it != it_end ; ++it)
      {
         StageClockListenerPtrPair & pair = *it;
         
         add (*pair.second, pair.first);
      }
      
      _latent_insertion_clock_listener_ptr_list.clear ();
   }
   
   if (!_latent_removal_clock_listener_ptr_list.empty ())
   {
      ClockListenerPtrList::iterator it
         = _latent_removal_clock_listener_ptr_list.begin ();

      const ClockListenerPtrList::iterator it_end
         = _latent_removal_clock_listener_ptr_list.end ();
      
      for (; it != it_end ; ++it)
      {
         ClockListener * listener_ptr = *it;
         
         remove (*listener_ptr);
      }
      
      _latent_removal_clock_listener_ptr_list.clear ();
   }
}



/*
==============================================================================
Name : is_in_latent_removal_state
==============================================================================
*/

bool  Clock::is_in_latent_removal_state (ClockListener * listener_ptr)
{
   ClockListenerPtrList::iterator it
      = _latent_removal_clock_listener_ptr_list.begin ();

   const ClockListenerPtrList::iterator it_end
      = _latent_removal_clock_listener_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      ClockListener * sub_listener_ptr = *it;
      
      if (listener_ptr == sub_listener_ptr)
      {
         return true;
      }
   }
   
   return false;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
