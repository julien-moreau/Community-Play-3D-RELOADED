/*****************************************************************************

        ObserverLockFree.h
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



#if ! defined (ohm_task_ObserverLockFree_HEADER_INCLUDED)
#define  ohm_task_ObserverLockFree_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/LockFreeQueue.h"
#include "ohm/conc/CellPool.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/ThreadSync.h"

#include <vector>
#include <typeinfo>



namespace ohm
{
namespace task
{



class Observer;

template <class T, class EventType>
class ObserverLockFree
:  sys::ThreadBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  ObserverLockFree (const char * debug_name_0, Observer & observer_fw, EventType event_type_fw);
   virtual        ~ObserverLockFree ();
   
   void           expand_to (size_t nbr_event);
   
   const char *   get_debug_name_0 () const;
   
   bool           send (T & event);
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // sys::ThreadBase
   virtual void   do_run ();
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef typename conc::LockFreeQueue <T>::CellType Cell;
   
   const char * const
                  _debug_name_0;
   Observer &     _observer_fw;
   const EventType
                  _event_type_fw;

   conc::LockFreeQueue <T>
                  _queue;
   conc::CellPool <T>
                  _cell_pool;
   sys::ThreadSync
                  _thread_sync;
   volatile bool  _stop_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObserverLockFree ();
                  ObserverLockFree (const ObserverLockFree &other);
   ObserverLockFree &      operator = (const ObserverLockFree &other);
   bool           operator == (const ObserverLockFree &other);
   bool           operator != (const ObserverLockFree &other);

}; // class ObserverLockFree



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/ObserverLockFree.hpp"



#endif   // ohm_task_ObserverLockFree_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
