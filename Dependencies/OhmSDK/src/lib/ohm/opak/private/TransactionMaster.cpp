/*****************************************************************************

        TransactionMaster.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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

#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/opal/private/Clock.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TransactionMaster::~TransactionMaster ()
{
}



/*
==============================================================================
Name : use
==============================================================================
*/

Transaction &  TransactionMaster::use ()
{
   return use_instance ().do_use ();
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  TransactionMaster::send ()
{
   if (!_main_thread_transaction.empty ())
   {
      Cell * cell_ptr = _cell_pool.take_cell (true);
      assert (cell_ptr != 0);

      cell_ptr->_val._transaction = _main_thread_transaction;

      _queue.enqueue (*cell_ptr);
      
      _main_thread_transaction.clear ();
   }
}



/*
==============================================================================
Name : send_wait_vbr
==============================================================================
*/

void  TransactionMaster::send_wait_vbr ()
{
   opal::VideoThread::use_instance ().request_vbr ();
   
   send ();
   
   opal::VideoThread::use_instance ().wait_vbr ();
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  TransactionMaster::init (double time_s)
{
   // clean up
   
   Cell * cell_ptr = 0;
   
   do
   {
      cell_ptr = _queue.dequeue ();
      
      if (cell_ptr != 0)
      {
         ohm_util_TRACE_ERROR ("opak Transaction dropped");
         
         _cell_pool.return_cell (*cell_ptr);
      }
   }
   while (cell_ptr != 0);
}



/*
==============================================================================
Name : tick
==============================================================================
*/

void  TransactionMaster::tick (double timestamp)
{
   receive ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

TransactionMaster::TransactionMaster ()
:  _queue ()
,  _cell_pool ()
{
   opal::Clock::use_instance ().add (*this, 0);
   
   _cell_pool.expand_to (128);
   
   _main_thread_id = sys::ThreadMgr::get_current_thread_id ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_use
==============================================================================
*/

Transaction &  TransactionMaster::do_use ()
{
   sys::ThreadMgr::IdType cur_thread_id
      = sys::ThreadMgr::get_current_thread_id ();
   
   if (cur_thread_id == _main_thread_id)
   {
      return _main_thread_transaction;
   }
   else
   {
      // calling from an unknown thread
      
      assert (false);
      return _main_thread_transaction;
   }
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  TransactionMaster::receive ()
{
   Cell * cell_ptr = 0;
   
   do
   {
      cell_ptr = _queue.dequeue ();
      
      if (cell_ptr != 0)
      {
         cell_ptr->_val._transaction.run ();
         
         _cell_pool.return_cell (*cell_ptr);
      }
   }
   while (cell_ptr != 0);
}
   


/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
