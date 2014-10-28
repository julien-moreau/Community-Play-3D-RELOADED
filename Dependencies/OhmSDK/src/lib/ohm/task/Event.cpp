/*****************************************************************************

        Event.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44889

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

#include "ohm/conc/AioAdd.h"
#include "ohm/conc/AtomicIntOp.h"
#include "ohm/conc/CellPool.h"
#include "ohm/task/Event.h"
#include "ohm/task/private/EventProvider.h"

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get
Description :
   Returns a new event.
Note :
   The event will return itself to it self pool.
==============================================================================
*/

Event &  Event::get (const char * name_0)
{
   return EventProvider::use_instance ().get_event (name_0);
}
   


/*
==============================================================================
Name : ctor
Note :
   Copy ctor is used when returning a reply event.
   We maitain the ref count of this event to know when to return the cell
   to its cell pool.
==============================================================================
*/

Event::Event (const Event & other)
:  _name_0 (other._name_0)
,  _cell_ptr (other._cell_ptr)
,  _count_ptr (other._count_ptr)
,  _data_sptr (other._data_sptr)
,  _read_pos (0)
{
   // not supposed to be read at this stage
   assert (other._read_pos == 0);
   assert (other._count_ptr != 0);
   assert (other._cell_ptr != 0);
   assert (other._name_0 != 0);
   
   const conc::AioAdd <long> increment (1);
   conc::AtomicIntOp_exec (*_count_ptr, increment);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Event::~Event ()
{
   if (_cell_ptr != 0)
   {
      assert (_count_ptr != 0);
      assert (*_count_ptr > 0);
      
      const conc::AioAdd <long> decrement (-1);
      long count = conc::AtomicIntOp_exec_new (*_count_ptr, decrement);

      if (count == 1)
      {
         return_cell ();
      }
   }
   else
   {
      // this was an unused cell from the cell pool
   }
}



/*
==============================================================================
Name : operator =
Note :
   see copy ctor
==============================================================================
*/

Event &  Event::operator = (const Event & other)
{
   assert (_cell_ptr == 0);
   assert (_read_pos == 0);
   
   // not supposed to be read at this stage
   assert (other._read_pos == 0);
   assert (other._count_ptr != 0);
   assert (other._cell_ptr != 0);
   assert (other._name_0 != 0);
   
   _name_0 = other._name_0;
   _cell_ptr = other._cell_ptr;
   _data_sptr = other._data_sptr;
   _count_ptr = other._count_ptr;
   
   const conc::AioAdd <long> increment (1);
   conc::AtomicIntOp_exec (*_count_ptr, increment);

   return *this;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Note :
   We need default ctor so that it can be use as a _val for a cell in a cell
   bool, but mark it invalid to prevent incorrect use.
==============================================================================
*/

Event::Event ()
:  _name_0 (0)
,  _cell_ptr (0)  // mark as invalid
,  _count_ptr (0)
,  _data_sptr (new std::vector <archi::UByte>)
,  _read_pos (0)
{
}



/*
==============================================================================
Name : bind_cell
==============================================================================
*/

void  Event::bind_cell (void * cell_ptr, const char * name_0)
{
   assert (_cell_ptr == 0);
   assert (cell_ptr != 0);
   
   _name_0 = name_0;
   _cell_ptr = cell_ptr;
   _count_ptr = new conc::AtomicInt <long> (1);
}



/*
==============================================================================
Name : return_cell
Note :
   The cell holding the event might not be this particular event.
   Rather, we let the event provider find the original event derived
   from the cell, and let it call 'clean_cell' on it.
==============================================================================
*/

void  Event::return_cell ()
{
   assert (*_count_ptr == 1);
   
   EventProvider::use_instance ().return_event (*this);
}



/*
==============================================================================
Name : clean_cell
==============================================================================
*/

void  Event::clean_cell ()
{
   assert (*_count_ptr == 1);
   
   delete _count_ptr;
   _count_ptr = 0;
   
   _data_sptr->clear ();
   _read_pos = 0;
   
   _name_0 = 0;
   _cell_ptr = 0;
}



/*
==============================================================================
Name : get_cell
==============================================================================
*/

void *   Event::get_cell () const
{
   assert (_cell_ptr != 0);
   
   return _cell_ptr;
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

const char *   Event::get_name () const
{
   assert (_name_0 != 0);
   
   return _name_0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
