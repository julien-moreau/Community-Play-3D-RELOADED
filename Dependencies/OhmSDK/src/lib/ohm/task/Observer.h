/*****************************************************************************

        Observer.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45010

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



#if ! defined (ohm_task_Observer_HEADER_INCLUDED)
#define  ohm_task_Observer_HEADER_INCLUDED

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

class Observer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  Observer (const char * debug_name_0);
   virtual        ~Observer ();
   
   const char *   get_debug_name_0 () const;
   
   void           set_order (float order);
   float          get_order () const;
   
   // at the begining of the run
   template <class ReceiverType, void (ReceiverType::*enter_process_method) ()>
   void           bind_enter_process (ReceiverType & receiver);
   // at the end of the run
   template <class ReceiverType, void (ReceiverType::*exit_process_method) ()>
   void           bind_exit_process (ReceiverType & receiver);

   // at the begining of each loop
   template <class ReceiverType, void (ReceiverType::*pre_process_method) ()>
   void           bind_pre_process (ReceiverType & receiver);
   // at the end of each loop
   template <class ReceiverType, void (ReceiverType::*post_process_method) ()>
   void           bind_post_process (ReceiverType & receiver);

   // event reception (eventually called many time at each loop)
   template <class ReceiverType, void (ReceiverType::*process_method) (Event &)>
   void           bind_process (ReceiverType & receiver);

   void           send (Event & event);
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // called by 'RunLoopBase'
   void           attach (RunLoopBase & run_loop);
   void           detach (RunLoopBase & run_loop);
   void           receive_enter ();
   void           receive_exit ();
   void           receive_pre ();
   void           receive_post ();
   void           receive_event (Event & event);

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Phase
   {
                  Phase_ENTER = 0,
                  Phase_EXIT,
                  
                  Phase_PRE,
                  Phase_POST,
                  
                  Phase_EVENT,
                  
                  Phase_NBR_ELT
   };
   
   const char * const
                  _debug_name_0;
   RunLoopBase *  _run_loop_ptr;

   float          _order;
   
   util::Delegate _delegate_arr [Phase_NBR_ELT];
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Observer ();
                  Observer (const Observer &other);
   Observer &     operator = (const Observer &other);
   bool           operator == (const Observer &other);
   bool           operator != (const Observer &other);

}; // class Observer



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/Observer.hpp"



#endif   // ohm_task_Observer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
