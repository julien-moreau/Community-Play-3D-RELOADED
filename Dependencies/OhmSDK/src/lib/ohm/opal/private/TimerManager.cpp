/*****************************************************************************

        TimerManager.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59437

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

#include "ohm/opal/private/TimerManager.h"
#include "ohm/opal/Event.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

TimerManager::TimerManager ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TimerManager::~TimerManager ()
{
}



/*
==============================================================================
Name : install_regular_timer
==============================================================================
*/

void  TimerManager::install_regular_timer (util::Delegate & delegate, double time)
{
   Info info;
   info._start_time = time;
   info._delay = -1.0;
   
   if (!_notifying_flag)
   {
      _timer_set [delegate] = info;
   }
   else
   {
      _latent_insertion_timer_list.push_back (std::make_pair (delegate, info));
   }
}



/*
==============================================================================
Name : install_one_shot_timer
==============================================================================
*/

void  TimerManager::install_one_shot_timer (util::Delegate & delegate, double delay, double time)
{
   assert (delay > 0.0);
   
   Info info;
   info._start_time = time;
   info._delay = delay;
   
   if (!_notifying_flag)
   {
      // be cool if the timer already exists
      _timer_set [delegate] = info;
   }
   else
   {
      _latent_insertion_timer_list.push_back (std::make_pair (delegate, info));
   }
}



/*
==============================================================================
Name : remove_timer
==============================================================================
*/

void  TimerManager::remove_timer (util::Delegate & delegate)
{
   if (!_notifying_flag)
   {
      TimerInfoSet::iterator it = _timer_set.find (delegate);
      
      // let's be cool if it exists or not
      
      if (it != _timer_set.end ())
      {
         _timer_set.erase (it);
      }
   }
   else
   {
      _latent_removal_timer_set.insert (delegate);
   }
}



/*
==============================================================================
Name : remove_timers
==============================================================================
*/

void  TimerManager::remove_timers (void * object_ptr)
{
   TimerInfoSet::iterator it = _timer_set.begin ();
   const TimerInfoSet::iterator it_end = _timer_set.end ();
   
   for (; it != it_end ;)
   {
      TimerInfoSet::iterator next_it = it;
      ++next_it;
      
      util::Delegate d = it->first;
      
      if (d.get_receiver_ptr () == object_ptr)
      {
         remove_timer (d);
      }
      
      it = next_it;
   }
}



/*
==============================================================================
Name : tick
==============================================================================
*/

void  TimerManager::tick (Event & event)
{
   _notifying_flag = true;
   
   {
      TimerInfoSet::iterator it = _timer_set.begin ();
      const TimerInfoSet::iterator it_end = _timer_set.end ();
      
      for (; it != it_end ; ++it)
      {
         util::Delegate delegate = it->first;
         Info & info = it->second;
         
         if (info._delay < 0.0)
         {
            event._timer._start_time = info._start_time;
         
            delegate.param_ref_signal (event);
         }
         else
         {
            if (info._start_time + info._delay < event._timer._time)
            {
               event._timer._start_time = info._start_time;
               
               delegate.param_ref_signal (event);
               
               remove_timer (delegate);
            }
         }
      }
   }
   
   _notifying_flag = false;
   
   if (!_latent_insertion_timer_list.empty ())
   {
      TimerList::iterator it = _latent_insertion_timer_list.begin ();
      const TimerList::iterator it_end = _latent_insertion_timer_list.end ();
      
      for (; it != it_end ; ++it)
      {
         util::Delegate & delegate = it->first;
         Info & info = it->second;
         
         _timer_set [delegate] = info;
      }
      
      _latent_insertion_timer_list.clear ();
   }
   
   if (!_latent_removal_timer_set.empty ())
   {
      TimerSet::iterator it = _latent_removal_timer_set.begin ();
      const TimerSet::iterator it_end = _latent_removal_timer_set.end ();
      
      for (; it != it_end ; ++it)
      {
         const util::Delegate & delegate = *it;
         
         TimerInfoSet::iterator fit = _timer_set.find (delegate);
         
         if (fit != _timer_set.end ())
         {
            _timer_set.erase (fit);
         }
      }
      
      _latent_removal_timer_set.clear ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
