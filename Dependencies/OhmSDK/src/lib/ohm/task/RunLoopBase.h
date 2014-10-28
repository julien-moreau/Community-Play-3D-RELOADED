/*****************************************************************************

        RunLoopBase.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49378

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



#if ! defined (ohm_task_RunLoopBase_HEADER_INCLUDED)
#define  ohm_task_RunLoopBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/LockFreeQueue.h"
#include "ohm/conc/CellPool.h"
#include "ohm/conc/AtomicInt.h"
#include "ohm/sys/ReadWriteLock.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/Event.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/util/Delegate.h"

#include <list>
#include <vector>
#include <map>
#include <typeinfo>



namespace ohm
{
namespace task
{



class RunLoopManager;
class Observer;
class Timer;

class RunLoopBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~RunLoopBase ();
   
   static RunLoopBase &
                  get_current ();
   
   void           add (Observer & observer);
   void           remove (Observer & observer);

   void           add (Timer & timer);
   void           remove (Timer & timer);
   
   bool           run ();
   void           stop ();
   
   bool           is_processing_event () const;
   
   virtual double get_time () = 0;
   
   void           release ();
   void           drop_latent_events ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  RunLoopBase ();

   friend class RunLoopManager;
   // called from 'RunLoopManager'
   void           send (Event & event);
   void           timer_fired (Timer * timer_ptr);
   
   // implemented by subclasses
   virtual bool   do_run () = 0;
   virtual void   do_stop () = 0;
   virtual void   do_signal () = 0;
   virtual void   do_update_time () = 0;
   
   // called from subclasses
   bool           process_pre ();
   void           process ();
   void           process_post ();
   
   // called from Clock



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum EventType
   {
                  EventType_EVENT = 0,
                  // Observer * : target observer
                  
                  EventType_TIMER_FIRED,
                  // Timer * : target timer
                  
                  EventType_STOP,
                  // no parameter
   };
   
   typedef conc::LockFreeQueue <Event>::CellType Cell;
   
   class ObserverBnd
   {
   public:
      enum State
      {
                  State_ADDED = 0,
                  State_RESIDENT,
                  State_REMOVED,
      };
      
      bool        operator < (const ObserverBnd & other) const;
      
      Observer *  _observer_ptr;
      State       _state;
   };
   
   typedef std::list <ObserverBnd>  ObserverBndArr;
   
   class TimerBnd
   {
   public:
      enum State
      {
                  State_ADDED = 0,
                  State_RESIDENT,
                  State_REMOVED,
      };
      
      Timer *     _timer_ptr;
      State       _state;
   };
   
   typedef std::list <TimerBnd>  TimerBndArr;
   
   void           enqueue (Event & event);

   void           increment_internal_msg ();
   void           decrement_internal_msg ();
   
   void           process_event (Event & event);
   void           process_timer_fired (Event & event);
   void           process_stop (Event & event);
   void           sync_observers ();
   void           call_observers_pre ();
   void           call_observers_post ();
   
   sys::ThreadMgr::IdType
                  _thread_id;
   ObserverBndArr _observer_bnd_arr;
   TimerBndArr    _timer_bnd_arr;
   bool           _first_run_flag;
   
   sys::ThreadSync
                  _stop_thread_sync;
   volatile bool  _stop_thread_sync_flag;
   
   bool           _processing_event_flag;
   
   conc::AtomicInt <long>
                  _nbr_internal_msg;
   
   conc::LockFreeQueue <Event>
                  _queue;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RunLoopBase (const RunLoopBase &other);
   RunLoopBase &  operator = (const RunLoopBase &other);
   bool           operator == (const RunLoopBase &other);
   bool           operator != (const RunLoopBase &other);

}; // class RunLoopBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/RunLoopBase.hpp"



#endif   // ohm_task_RunLoopBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
