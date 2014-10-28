/*****************************************************************************

        Timer.h
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



#if ! defined (ohm_task_Timer_HEADER_INCLUDED)
#define  ohm_task_Timer_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/LockFreeQueue.h"
#include "ohm/conc/CellPool.h"
#include "ohm/sys/ReadWriteLock.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/Event.h"
#include "ohm/util/Delegate.h"

#include <vector>
#include <typeinfo>



namespace ohm
{
namespace task
{



class RunLoopBase;

class Timer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  Timer (const char * debug_name_0);
   virtual        ~Timer ();
   
   void           set_periodic (double period_s);
   bool           is_periodic () const;
   double         get_period () const;
   
   void           set_one_shot (double delay_s);
   bool           is_one_shot () const;
   double         get_delay () const;
   
   double         get_time_value ();
   
   const char *   get_debug_name_0 () const;
   
   // event reception (eventually called many time at each loop)
   template <class ReceiverType, void (ReceiverType::*process_method) ()>
   void           bind_process (ReceiverType & receiver);
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // called by 'RunLoopBase'
   void           attach (RunLoopBase & run_loop);
   void           detach (RunLoopBase & run_loop);
   void           receive_fire ();

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Kind
   {
                  Kind_NONE = 0,
                  Kind_PERIODIC,
                  Kind_ONE_SHOT,
   };
   
   const char * const
                  _debug_name_0;
   RunLoopBase *  _run_loop_ptr;
   
   Kind           _kind;
   double         _time_s;
   
   util::Delegate _delegate;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Timer ();
                  Timer (const Timer &other);
   Timer &     operator = (const Timer &other);
   bool           operator == (const Timer &other);
   bool           operator != (const Timer &other);

}; // class Timer



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/Timer.hpp"



#endif   // ohm_task_Timer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
