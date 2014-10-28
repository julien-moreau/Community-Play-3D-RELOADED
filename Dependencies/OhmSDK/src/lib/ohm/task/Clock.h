/*****************************************************************************

        Clock.h
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



#if ! defined (ohm_task_Clock_HEADER_INCLUDED)
#define  ohm_task_Clock_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"
#include "ohm/conc/CellPool.h"
#include "ohm/conc/LockFreeQueue.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/util/SingletonBase.h"
#include "ohm/util/Delegate.h"

#include <map>



namespace ohm
{
namespace task
{



class RunLoopBase;
class Timer;

class Clock
:  public util::SingletonBase <Clock>
,  public sys::ThreadBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~Clock ();
   
   void           release ();
   
   void           bind_alarm (RunLoopBase * run_loop_ptr, Timer * timer_ptr, double delay_s);
   void           unbind_alarm (RunLoopBase * run_loop_ptr, Timer * timer_ptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  Clock ();
   
   // sys::ThreadBase
   virtual void   do_run ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (Clock);
   
   typedef std::pair <RunLoopBase *, Timer *>   RunLoopTimer;
   
   class Event
   {
   public:
      enum Type
      {
                  Type_ADD = 0,
                  Type_REMOVE,
      };
      
      Type        _type;
      double      _due_time;
      RunLoopTimer
                  _run_loop_timer;
   };
   
   typedef std::multimap <double, RunLoopTimer> AlarmMap;
   typedef conc::CellPool <Event>::CellType Cell;
   
   void           process_queue ();
   void           process_add (Event & event);
   void           process_remove (Event & event);
   
   AlarmMap       _alarm_map;
   
   sys::ThreadSync
                  _queue_sync;
   conc::CellPool <Event>
                  _cell_pool;
   conc::LockFreeQueue <Event>
                  _queue;

   volatile bool  _exit_request_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Clock (const Clock &other);
   Clock &        operator = (const Clock &other);
   bool           operator == (const Clock &other) const;
   bool           operator != (const Clock &other) const;

}; // class Clock



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/Clock.hpp"



#endif   // ohm_task_Clock_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
