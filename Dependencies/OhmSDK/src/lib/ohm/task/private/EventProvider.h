/*****************************************************************************

        EventProvider.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62950

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



#if ! defined (ohm_task_EventProvider_HEADER_INCLUDED)
#define  ohm_task_EventProvider_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/CellPool.h"
#include "ohm/util/SingletonBase.h"
#include "ohm/sys/Mutex.h"
#include "ohm/task/Event.h"

#include <set>



#undef OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG

#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG is defined in shipping level code !
   #endif
#endif



namespace ohm
{
namespace task
{



class Event;

class EventProvider
:  public ohm::util::SingletonBase <EventProvider>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~EventProvider ();
   
   Event &        get_event (const char * name_0);
   void           return_event (Event & event);

   void           release ();


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  EventProvider ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (EventProvider);
   
   typedef conc::CellPool <Event> CellPool;
   typedef conc::CellPool <Event>::CellType  Cell;
   
   CellPool       _cell_pool;

#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
   sys::Mutex     _out_cell_ptr_set_mutex;
   std::set <Cell *>
                  _out_cell_ptr_set;
   bool           _released_flag;
#endif



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventProvider (const EventProvider &other);
   EventProvider &operator = (const EventProvider &other);
   bool           operator == (const EventProvider &other) const;
   bool           operator != (const EventProvider &other) const;

}; // class EventProvider



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/EventProvider.hpp"



#endif   // ohm_task_EventProvider_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
