/*****************************************************************************

        ObserverLockFree.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49593

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



#if defined (ohm_task_ObserverLockFree_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObserverLockFree code header.
#endif
#define  ohm_task_ObserverLockFree_CURRENT_CODEHEADER

#if ! defined (ohm_task_ObserverLockFree_CODEHEADER_INCLUDED)
#define  ohm_task_ObserverLockFree_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/Event.h"



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T, class EventType>
ObserverLockFree <T, EventType>::ObserverLockFree (const char * debug_name_0, Observer & observer_fw, EventType event_type_fw)
:  _debug_name_0 (debug_name_0)
,  _observer_fw (observer_fw)
,  _event_type_fw (event_type_fw)
,  _queue ()
,  _cell_pool ()
,  _thread_sync ()
,  _stop_flag (false)
{
   sys::ThreadBase::run ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T, class EventType>
ObserverLockFree <T, EventType>::~ObserverLockFree ()
{
   try
   {
      // stop
      
      _stop_flag = true;
      _thread_sync.signal ();
      
      sys::ThreadBase::wait_for_death ();
      
      // returns the cells to the cell pool
      
      Cell * cell_ptr = 0;
      
      size_t nbr_event = 0;
      
      do
      {
         cell_ptr = _queue.dequeue ();
         
         if (cell_ptr != 0)
         {
            _cell_pool.return_cell (*cell_ptr);
            ++nbr_event;
         }
      }
      while (cell_ptr != 0);
      
      if (nbr_event > 0)
      {
         ohm_util_TRACE_ERROR2 ("%1% : %2% events dropped", typeid (*this), nbr_event);
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : expand_to
Description :
   Expand the number of events that can be simultaneously managed by
   the observer.
   E.g. sending too much events before they can be read, if going more
   than this nbr of event will make the event to be dropped.
Note :
   This is thread safe but locks.
==============================================================================
*/

template <class T, class EventType>
void  ObserverLockFree <T, EventType>::expand_to (size_t nbr_event)
{
   _cell_pool.expand_to (nbr_event);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T, class EventType>
const char *   ObserverLockFree <T, EventType>::get_debug_name_0 () const
{
   return _debug_name_0;
}



/*
==============================================================================
Name : send
Description :
   Send the event. Returns true if event was sent, false otherwise.
Note :
   To avoid locks, if there are no more room for one more event, we will
   drop the event.
==============================================================================
*/

template <class T, class EventType>
bool  ObserverLockFree <T, EventType>::send (T & event)
{
   bool autogrow_flag = false;
   
   Cell * cell_ptr = _cell_pool.take_cell (autogrow_flag);
   
   bool ok_flag = cell_ptr != 0;
   
   if (ok_flag)
   {
      cell_ptr->_val = event;
      
      _queue.enqueue (*cell_ptr);
      
      _thread_sync.signal ();
   }
   
   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_run
==============================================================================
*/

template <class T, class EventType>
void  ObserverLockFree <T, EventType>::do_run ()
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      _thread_sync.wait ();
      
      if (_stop_flag)
      {
         break;
      }
      
      Cell * cell_ptr = 0;
      
      do
      {
         cell_ptr = _queue.dequeue ();
         
         if (cell_ptr != 0)
         {
            T & event = cell_ptr->_val;
            
            EventType event_type = _event_type_fw;
            
            task::Event & fw_event = ohm_task_EVENT_GET;
            fw_event.push (event_type);
            fw_event.push (event);
            
            _observer_fw.send (fw_event); 
            
            _cell_pool.return_cell (*cell_ptr);
         }
      }
      while (cell_ptr != 0);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace task
}  // namespace ohm



#endif   // ohm_task_ObserverLockFree_CODEHEADER_INCLUDED

#undef ohm_task_ObserverLockFree_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

