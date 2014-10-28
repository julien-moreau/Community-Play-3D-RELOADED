/*****************************************************************************

        Clock.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68831

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

#include "ohm/task/Clock.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/sys/TimerReliable.h"

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Clock::~Clock ()
{
   release ();

   if (! wait_for_death (1.0))
   {
      kill ();
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  Clock::release ()
{
   _exit_request_flag = true;
   _queue_sync.signal ();
}



/*
==============================================================================
Name : bind_alarm
Note :
   Called from 'run_loop_ptr' thread.
==============================================================================
*/

void  Clock::bind_alarm (RunLoopBase * run_loop_ptr, Timer * timer_ptr, double delay_s)
{
   RunLoopTimer run_loop_timer = std::make_pair (run_loop_ptr, timer_ptr);
   
   double now = sys::TimerReliable::use_instance ().get ();
   
   Cell * cell_ptr = _cell_pool.take_cell (true);
   assert (cell_ptr != 0);
   
   Event & event = cell_ptr->_val;
   
   event._type = Event::Type_ADD;
   event._due_time = now + delay_s;
   event._run_loop_timer = run_loop_timer;
   
   _queue.enqueue (*cell_ptr);
   
   _queue_sync.signal ();
}



/*
==============================================================================
Name : unbind_alarm
Note :
   Called from 'run_loop_ptr' thread.
==============================================================================
*/

void  Clock::unbind_alarm (RunLoopBase * run_loop_ptr, Timer * timer_ptr)
{
   RunLoopTimer run_loop_timer = std::make_pair (run_loop_ptr, timer_ptr);
   
   Cell * cell_ptr = _cell_pool.take_cell (true);
   assert (cell_ptr != 0);
   
   Event & event = cell_ptr->_val;
   
   event._type = Event::Type_REMOVE;
   event._run_loop_timer = run_loop_timer;
   
   _queue.enqueue (*cell_ptr);
   
   _queue_sync.signal ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Clock::Clock ()
:  _alarm_map ()
,  _cell_pool ()
,  _queue ()
,  _exit_request_flag (false)
{
   _cell_pool.expand_to (2);
   
   ThreadBase::run ();
}



/*
==============================================================================
Name : do_run
==============================================================================
*/

void  Clock::do_run ()
{
   while (! _exit_request_flag)
   {
      process_queue ();
      
      double timeout = -1;
      
      if (!_alarm_map.empty ())
      {
         double now = sys::TimerReliable::use_instance ().get ();
      
         double first = _alarm_map.begin ()->first;
         
         timeout = lang::max (0.0, first - now);
      }
      
      _queue_sync.wait (timeout);

      if (! _exit_request_flag)
      {
         double now = sys::TimerReliable::use_instance ().get ();
         
         AlarmMap::iterator it = _alarm_map.begin ();
         AlarmMap::iterator it_end = _alarm_map.end ();
         
         for (; it != it_end ;)
         {
            AlarmMap::iterator it_next = it;
            ++it_next;

            double due_time = it->first;
            
            if (due_time <= now)
            {
               RunLoopTimer run_loop_timer = it->second;
               
               RunLoopManager::use_instance ().timer_fired (run_loop_timer);
               
               _alarm_map.erase (it);
            }
            else
            {
               break;
            }
            
            it = it_next;
         }
      }
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_queue
==============================================================================
*/

void  Clock::process_queue ()
{
   Cell * cell_ptr = 0;
   
   do
   {
      cell_ptr = _queue.dequeue ();
      
      if (cell_ptr != 0)
      {
         Event & event = cell_ptr->_val;
         
         switch (event._type)
         {
         case Event::Type_ADD:
            process_add (event);
            break;

         case Event::Type_REMOVE:
            process_remove (event);
            break;
         
         default:
            assert (false);
            break;
         }
         
         _cell_pool.return_cell (*cell_ptr);
      }
   }
   while (cell_ptr != 0);
}



/*
==============================================================================
Name : process_add
==============================================================================
*/

void  Clock::process_add (Event & event)
{
   _alarm_map.insert (std::make_pair (
      event._due_time, event._run_loop_timer
   ));
}



/*
==============================================================================
Name : process_remove
==============================================================================
*/

void  Clock::process_remove (Event & event)
{
   AlarmMap::iterator it = _alarm_map.begin ();
   const AlarmMap::iterator it_end = _alarm_map.end ();
   
   for (; it != it_end ;)
   {
      AlarmMap::iterator it_next = it;
      ++it_next;
      
      const RunLoopTimer & sub_run_loop_timer = it->second;
      
      if (sub_run_loop_timer == event._run_loop_timer)
      {
         _alarm_map.erase (it);
      }
      
      it = it_next;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
