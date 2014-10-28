/*****************************************************************************

        Port.cpp
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

#include "ohm/task/Port.h"

#include <cassert>



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

Port::Port ()
:  _queue ()
,  _sync ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Port::~Port ()
{
   Cell * cell_ptr = 0;
   
   do
   {
      cell_ptr = _queue.dequeue ();
      
      if (cell_ptr != 0)
      {
         Event & event = cell_ptr->_val;
         
         ohm_util_TRACE_INFO ("event dropped in port dtor");
         
         event.return_cell ();
      }
   }
   while (cell_ptr != 0);
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  Port::send (Event & event)
{
   Cell * cell_ptr = reinterpret_cast <Cell *> (event.get_cell ());
   assert (cell_ptr != 0);
   
   _queue.enqueue (*cell_ptr);

   _sync.signal ();
}



/*
==============================================================================
Name : recv
Description :
   Block until an event is received and return the event.
==============================================================================
*/

Event Port::recv ()
{
   Event ret_val;
   
   bool ok_flag = recv (ret_val);
   assert (ok_flag);
   
   return ret_val;
}



/*
==============================================================================
Name : recv
Description :
   Block until an event is received or 'timeout' is reached.
   Returns true is an event was received, false if the wait timed out.
==============================================================================
*/

bool  Port::recv (Event & event, double timeout)
{
   bool timeout_flag = _sync.wait (timeout);
   
   if (!timeout_flag)
   {
      Cell * cell_ptr = _queue.dequeue ();
      assert (cell_ptr != 0);
      
      event = cell_ptr->_val;
   }
   
   return !timeout_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
