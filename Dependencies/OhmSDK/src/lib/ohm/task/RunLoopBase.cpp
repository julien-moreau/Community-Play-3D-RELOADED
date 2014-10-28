/*****************************************************************************

        RunLoopBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59158

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

#include "ohm/task/RunLoopBase.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/task/Observer.h"
#include "ohm/task/Timer.h"
#include "ohm/util/trace.h"

#include <algorithm>

#include <cassert>



#undef OHM_SYS_RL_RUN_LOOP_BASE_DEBUG_FLAG

#if defined (OHM_SYS_RL_RUN_LOOP_BASE_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_SYS_RL_RUN_LOOP_BASE_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define rlb_TRACE(msg_0)   ohm_util_TRACE_INFO(msg_0)
   #define rlb_TRACE1(fmt_0, arg1)  ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define rlb_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define rlb_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define rlb_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define rlb_TRACE(msg_0)   (void (0))
   #define rlb_TRACE1(fmt_0, arg1)  (void (0))
   #define rlb_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define rlb_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define rlb_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
#endif



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

RunLoopBase::~RunLoopBase ()
{
   try
   {
      release ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : get_current
Description :
   Returns the run loop associated to the current running thread.
   Run loop might be nested. In that case the current run loop is the top of
   the pile.
Thread Safety :
   Can be called by any thread.
Note :
   Crash if there are no run loop associated to the current running thread.
==============================================================================
*/

RunLoopBase &  RunLoopBase::get_current ()
{
   return RunLoopManager::use_instance ().get_current ();
}



/*
==============================================================================
Name : add
Note :
   The observer can *only* be added from the same thread as the run loop
   thread. If the run loop is not already running, you'll have to ensure it.
==============================================================================
*/

void  RunLoopBase::add (Observer & observer)
{
   assert (&observer != 0);
#if ! defined (NDEBUG)
   if (_thread_id != sys::ThreadMgr::get_invalid_thread_id ())
   {
      assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   }
#endif
   
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ObserverBnd & observer_bnd = *it;

      if (observer_bnd._observer_ptr == &observer)
      {
         assert (observer_bnd._state == ObserverBnd::State_REMOVED);
         
         observer_bnd._state = ObserverBnd::State_ADDED;
         
         break;
      }
   }
   
   if (it == it_end)
   {
      ObserverBnd observer_bnd;
      observer_bnd._observer_ptr = &observer;
      observer_bnd._state = ObserverBnd::State_ADDED;
      
      _observer_bnd_arr.push_back (observer_bnd);
   }

   observer.attach (*this);
   observer.receive_enter ();
}



/*
==============================================================================
Name : remove
Note :
   The observer can *only* be removed from the same thread as the run loop
   thread.
==============================================================================
*/

void  RunLoopBase::remove (Observer & observer)
{
   assert (&observer != 0);
#if ! defined (NDEBUG)
   if (_thread_id != sys::ThreadMgr::get_invalid_thread_id ())
   {
      assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   }
#endif
   
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ObserverBnd & observer_bnd = *it;

      if (observer_bnd._observer_ptr == &observer)
      {
         assert (observer_bnd._state != ObserverBnd::State_REMOVED);
         
         observer_bnd._state = ObserverBnd::State_REMOVED;
         
         observer.receive_exit ();
         observer.detach (*this);
         
         break;
      }
   }
   
   assert (it != it_end);
}



/*
==============================================================================
Name : add
Note :
   The timer can *only* be added from the same thread as the run loop
   thread. If the run loop is not already running, you'll have to ensure it.
==============================================================================
*/

void  RunLoopBase::add (Timer & timer)
{
   assert (&timer != 0);
#if ! defined (NDEBUG)
   if (_thread_id != sys::ThreadMgr::get_invalid_thread_id ())
   {
      assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   }
#endif
   
   TimerBndArr::iterator it = _timer_bnd_arr.begin ();
   TimerBndArr::iterator it_end = _timer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      TimerBnd & timer_bnd = *it;

      if (timer_bnd._timer_ptr == &timer)
      {
         assert (timer_bnd._state == TimerBnd::State_REMOVED);
         
         timer_bnd._state = TimerBnd::State_ADDED;
         
         break;
      }
   }
   
   if (it == it_end)
   {
      TimerBnd timer_bnd;
      timer_bnd._timer_ptr = &timer;
      timer_bnd._state = TimerBnd::State_ADDED;
      
      _timer_bnd_arr.push_back (timer_bnd);
   }

   timer.attach (*this);
}



/*
==============================================================================
Name : remove
Note :
   The timer can *only* be removed from the same thread as the run loop
   thread.
==============================================================================
*/

void  RunLoopBase::remove (Timer & timer)
{
   assert (&timer != 0);
#if ! defined (NDEBUG)
   if (_thread_id != sys::ThreadMgr::get_invalid_thread_id ())
   {
      assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   }
#endif
   
   TimerBndArr::iterator it = _timer_bnd_arr.begin ();
   TimerBndArr::iterator it_end = _timer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      TimerBnd & timer_bnd = *it;

      if (timer_bnd._timer_ptr == &timer)
      {
         assert (timer_bnd._state != TimerBnd::State_REMOVED);
         
         timer_bnd._state = TimerBnd::State_REMOVED;
         
         timer.detach (*this);
         
         break;
      }
   }
   
   assert (it != it_end);
}



/*
==============================================================================
Name : run
Description :
   Run the run loop. Run loops derivates have different behavior :
   - RunLoopGui is meant for gui and will block until stop is called
   - RunLoop will block until stop is called
   - RunLoopOnce will run once and returns once it has processed. If stop
     was called, then it will return false.
   Returns false if the run loop received a stop msg, true otherwise.
Note :
   Run loop must not be migrated from one thread to another thread.
==============================================================================
*/

bool  RunLoopBase::run ()
{
   assert (&RunLoopManager::use_instance ().get_current () == this);
   
   if (_first_run_flag)
   {
      _thread_id = sys::ThreadMgr::get_current_thread_id ();
      
      _first_run_flag = false;
   }
   
   bool run_flag = do_run ();
   
   if (!run_flag)
   {
      _thread_id = sys::ThreadMgr::get_invalid_thread_id ();
      
      _first_run_flag = true;
   }
   
   if (_stop_thread_sync_flag)
   {
      _stop_thread_sync.signal ();
   }
   
   return run_flag;
}



/*
==============================================================================
Name : stop
Description :
   Request to stop the run loop.
   If called from run loop thread, this function returns immediatly.
   If called from another thread, this function will block until the run
   loop as returned.
==============================================================================
*/

void  RunLoopBase::stop ()
{
   assert (_thread_id != sys::ThreadMgr::get_invalid_thread_id ());
   
   _stop_thread_sync_flag = _thread_id != sys::ThreadMgr::get_current_thread_id ();

   Event & event = ohm_task_EVENT_GET;
   
   event.push (EventType_STOP);
   
   enqueue (event);
   
   if (_stop_thread_sync_flag)
   {
      _stop_thread_sync.wait ();
   }
}



/*
==============================================================================
Name : is_processing_event
Thread Safety :
   Can be called from any thread, but the value might not be exactly
   the one of the RunLoop.
==============================================================================
*/

bool  RunLoopBase::is_processing_event () const
{
   return _processing_event_flag;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  RunLoopBase::release ()
{
   if (!_observer_bnd_arr.empty ())
   {
      ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
      ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         ObserverBnd & observer_bnd = *it;
         
         assert (observer_bnd._state == ObserverBnd::State_REMOVED);
      }
   }
   
   drop_latent_events ();
}



/*
==============================================================================
Name : drop_latent_events
==============================================================================
*/

void  RunLoopBase::drop_latent_events ()
{
   Cell * cell_ptr = 0;
      
   do
   {
      cell_ptr = _queue.dequeue ();
      
      if (cell_ptr != 0)
      {
         decrement_internal_msg ();
         
         Event & event = cell_ptr->_val;
         
         ohm_util_TRACE_INFO1 ("event %1% dropped in run loop dtor", event.get_name ());
         
         event.return_cell ();
      }
   }
   while (cell_ptr != 0);
   
   assert (_nbr_internal_msg == 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

RunLoopBase::RunLoopBase ()
:  _thread_id (sys::ThreadMgr::get_invalid_thread_id ())
,  _observer_bnd_arr ()
,  _first_run_flag (true)
,  _processing_event_flag (false)
,  _nbr_internal_msg (0)
,  _queue ()
{
}



/*
==============================================================================
Name : send
Note :
   The sent event is prefixed with a pointer to the observer, which serves
   as an id.
==============================================================================
*/

void  RunLoopBase::send (Event & event)
{
   event.push_front (EventType_EVENT);
   
   enqueue (event);
}



/*
==============================================================================
Name : timer_fired
==============================================================================
*/

void  RunLoopBase::timer_fired (Timer * timer_ptr)
{
   task::Event & event = ohm_task_EVENT_GET;
   
   event.push (EventType_TIMER_FIRED);
   event.push (timer_ptr);
   
   enqueue (event);
}



/*
==============================================================================
Name : process_pre
Note :
   called before run loop process an event (like a gui event in the
   case of the RunLoopGui)
   Returns true if the processing can be done, false otherwise.
   In the case where it returns false, 'process' & 'process_post' must not be
   called.
Note :
   This is executed in the run loop thread.
==============================================================================
*/

bool  RunLoopBase::process_pre ()
{
   if (!_processing_event_flag)
   {
      _processing_event_flag = true;
      
      do_update_time ();
      
      sync_observers ();
      
      call_observers_pre ();
      
      return true;
   }
   else
   {
      return false;
   }
}



/*
==============================================================================
Name : process
Note :
   called when the run loop has been signalled. this is only in the case of
   a run loop event (ie. not an external event like an user input event for
   the RunLoopGui)
Note :
   This is executed in the run loop thread.
==============================================================================
*/

void  RunLoopBase::process ()
{
   assert (_processing_event_flag);
   
   Cell * cell_ptr = 0;
      
   do
   {
      cell_ptr = _queue.dequeue ();
      
      if (cell_ptr != 0)
      {
         decrement_internal_msg ();
         
         Event & event = cell_ptr->_val;
         
         EventType type = event.pop <EventType> ();
         
         switch (type)
         {
         case EventType_EVENT:
            process_event (event);
            break;

         case EventType_TIMER_FIRED:
            process_timer_fired (event);
            break;

         case EventType_STOP:
            process_stop (event);
            break;
         
         default:
            assert (false);
            break;
         }
         
         event.return_cell ();
      }
   }
   while (cell_ptr != 0);
}



/*
==============================================================================
Name : process_post
Note :
   called after run loop process an external event (like a gui event in the
   case of the RunLoopGui)
Note :
   This is executed in the run loop thread.
==============================================================================
*/

void  RunLoopBase::process_post ()
{
   assert (_processing_event_flag);

   call_observers_post ();
   
   _processing_event_flag = false;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : enqueue
==============================================================================
*/

void  RunLoopBase::enqueue (Event & event)
{
   Cell * cell_ptr = reinterpret_cast <Cell *> (event.get_cell ());
   assert (cell_ptr != 0);

   _queue.enqueue (*cell_ptr);
   
   // it is necessary to signal after the cell is queued
   increment_internal_msg ();
}



/*
==============================================================================
Name : increment_internal_msg
==============================================================================
*/

void  RunLoopBase::increment_internal_msg ()
{
   const conc::AioAdd <long>  ftor_inc (1);
   long old_nbr_internal_msg = conc::AtomicIntOp_exec_old (_nbr_internal_msg, ftor_inc);
   
   if (old_nbr_internal_msg <= 0)
   {
      do_signal ();
   }
}



/*
==============================================================================
Name : decrement_internal_msg
==============================================================================
*/

void  RunLoopBase::decrement_internal_msg ()
{
   const conc::AioAdd <long>  ftor_inc (-1);
   conc::AtomicIntOp_exec (_nbr_internal_msg, ftor_inc);
   
   // because we decrement after the cell is dequeued, we can
   // run into the situation where '_nbr_internal_msg' (new) would
   // be negative for a short time
}



/*
==============================================================================
Name : process_event
==============================================================================
*/

void  RunLoopBase::process_event (Event & event)
{
   Observer * observer_ptr = event.pop <Observer *> ();
         
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   const ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ObserverBnd & observer_bnd = *it;
      
      if (observer_bnd._state != ObserverBnd::State_REMOVED)
      {
         if (observer_bnd._observer_ptr == observer_ptr)
         {
            observer_ptr->receive_event (event);
            break;
         }
      }
   }
}



/*
==============================================================================
Name : process_timer_fired
==============================================================================
*/

void  RunLoopBase::process_timer_fired (Event & event)
{
   Timer * timer_ptr = event.pop <Timer *> ();
   
   TimerBndArr::iterator it = _timer_bnd_arr.begin ();
   const TimerBndArr::iterator it_end = _timer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      TimerBnd & timer_bnd = *it;
      
      if (timer_bnd._state != TimerBnd::State_REMOVED)
      {
         if (timer_bnd._timer_ptr == timer_ptr)
         {
            timer_ptr->receive_fire ();
            break;
         }
      }
   }
}



/*
==============================================================================
Name : process_stop
==============================================================================
*/

void  RunLoopBase::process_stop (Event & event)
{
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   const ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ObserverBnd & observer_bnd = *it;
      
      if (observer_bnd._state != ObserverBnd::State_REMOVED)
      {
         Observer * observer_ptr = observer_bnd._observer_ptr;

         observer_ptr->receive_exit ();
      }
   }
   
   do_stop ();
}



/*
==============================================================================
Name : sync_observers
==============================================================================
*/

void  RunLoopBase::sync_observers ()
{
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   bool sort_flag = false;
   
   for (; it != it_end ;)
   {
      ObserverBndArr::iterator it_next = it;
      ++it_next;
      
      ObserverBnd & observer_bnd = *it;

      if (observer_bnd._state == ObserverBnd::State_REMOVED)
      {
         _observer_bnd_arr.erase (it);
      }
      else if (observer_bnd._state == ObserverBnd::State_ADDED)
      {
         observer_bnd._state = ObserverBnd::State_RESIDENT;
         sort_flag = true;
      }
      
      it = it_next;
   }
   
   if (sort_flag)
   {
      _observer_bnd_arr.sort ();
   }
}



/*
==============================================================================
Name : call_observers_pre
==============================================================================
*/

void  RunLoopBase::call_observers_pre ()
{
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   const ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ObserverBnd & observer_bnd = *it;
      
      if (observer_bnd._state != ObserverBnd::State_REMOVED)
      {
         Observer * observer_ptr = observer_bnd._observer_ptr;
         
         observer_ptr->receive_pre ();
      }
   }
}



/*
==============================================================================
Name : call_observers_post
==============================================================================
*/

void  RunLoopBase::call_observers_post ()
{
   ObserverBndArr::iterator it = _observer_bnd_arr.begin ();
   const ObserverBndArr::iterator it_end = _observer_bnd_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ObserverBnd & observer_bnd = *it;
      
      if (observer_bnd._state != ObserverBnd::State_REMOVED)
      {
         Observer * observer_ptr = observer_bnd._observer_ptr;
         
         observer_ptr->receive_post ();
      }
   }
}



/*
==============================================================================
Name : ObserverBnd::operator <
==============================================================================
*/

bool  RunLoopBase::ObserverBnd::operator < (const ObserverBnd & other) const
{
   return _observer_ptr->get_order () < other._observer_ptr->get_order ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
