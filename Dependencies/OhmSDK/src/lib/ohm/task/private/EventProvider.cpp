/*****************************************************************************

        EventProvider.cpp
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

#include "ohm/task/private/EventProvider.h"

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

EventProvider::~EventProvider ()
{
#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
   if (!_released_flag)
   {
      release ();
   }
#endif

}



/*
==============================================================================
Name : get_event
==============================================================================
*/

Event &  EventProvider::get_event (const char * name_0)
{
   Cell * cell_ptr = _cell_pool.take_cell (true);
   assert (cell_ptr != 0);
   
   Event & event = cell_ptr->_val;
   event.bind_cell (cell_ptr, name_0);
   
#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
   _out_cell_ptr_set_mutex.lock ();
   assert (_out_cell_ptr_set.find (cell_ptr) == _out_cell_ptr_set.end ());
   _out_cell_ptr_set.insert (cell_ptr);
   _out_cell_ptr_set_mutex.unlock ();
#endif
   
   return event;
}



/*
==============================================================================
Name : return_event
==============================================================================
*/

void  EventProvider::return_event (Event & event)
{
   Cell * cell_ptr = reinterpret_cast <Cell *> (event.get_cell ());
   
   // original event from cell, we might have :
   // &event != &cell_event
   
   Event & cell_event = cell_ptr->_val;
   
   cell_event.clean_cell ();

#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
   _out_cell_ptr_set_mutex.lock ();
   assert (_out_cell_ptr_set.find (cell_ptr) != _out_cell_ptr_set.end ());
   _out_cell_ptr_set.erase (cell_ptr);
   _out_cell_ptr_set_mutex.unlock ();
#endif
   
   _cell_pool.return_cell (*cell_ptr);
}



/*
==============================================================================
Name: release
==============================================================================
*/

void EventProvider::release ()
{
#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
   if (!_out_cell_ptr_set.empty ())
   {
      ohm_util_TRACE_ERROR ("EventProvider has non returned events");
      
      std::set <Cell *>::iterator it = _out_cell_ptr_set.begin ();
      const std::set <Cell *>::iterator it_end = _out_cell_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Cell * cell_ptr = *it;
         
         Event & event = cell_ptr->_val;
         
         const char * name_0 = event.get_name ();
         
         ohm_util_TRACE_ERROR1 ("  from %1%", name_0);
      }
   }
   _released_flag = true;
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

EventProvider::EventProvider ()
:  _cell_pool ()
#if defined (OHM_TASK_EVENT_PROVIDER_DEBUG_FLAG)
,  _released_flag (false)
#endif
{
   _cell_pool.expand_to (32);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
