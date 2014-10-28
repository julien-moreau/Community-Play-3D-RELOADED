/*****************************************************************************

        ThreadBaseImpl.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67833

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

#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/macho/ThreadBaseImpl.h"
#include "ohm/util/trace.h"

#include <stdexcept>
#include <cassert>


namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ThreadBaseImpl::ThreadBaseImpl (ThreadBase * thread_ptr)
:  _task_id (0)
,  _thread_ptr (thread_ptr)
,  _wait_id (0)
{
   assert (_thread_ptr != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ThreadBaseImpl::~ThreadBaseImpl ()
{
   _task_id = 0;
   _thread_ptr = 0;
}



/*
==============================================================================
Name : run
==============================================================================
*/

int   ThreadBaseImpl::run ()
{
   init ();
   
   ::OSErr err = ::MPCreateTask (
      dispatch_and_run,
      this,          // param
      0, // stack size
      0,
      0,
      0,
      0,
      &_task_id
   );
   assert (err == ::noErr);
      
   return err;
}



/*
==============================================================================
Name : wait_for_death
==============================================================================
*/

bool  ThreadBaseImpl::wait_for_death (double timeout)
{
   assert (_task_id != 0);
   
   Duration duration = kDurationForever;
   
   if (timeout >= 0)
   {
      assert (timeout < 2000.0);
      double micro_second = timeout * 1000000.0;
      ::SInt32 micro_second_d = ::SInt32 (micro_second);
      
      duration = kDurationMicrosecond * micro_second_d;
   }
   
   OSStatus err = MPWaitOnSemaphore (_wait_id, duration);
   
   assert ((err == 0) || (err == kMPTimeoutErr));
   
   bool ret_val = false;
   
   if (err == 0)
   {
      _task_id = 0;
      ret_val = true;
   }
   
   restore ();
   
   return ret_val;
}



/*
==============================================================================
Name : kill
==============================================================================
*/

void  ThreadBaseImpl::kill ()
{
   if (_task_id != 0)
   {
      // the following call may fail if the 
      // thread no longer exists
      
      MPTerminateTask (
         _task_id,
         noErr
      );
      _task_id = 0;
   }
   
   restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dispatch_and_run
==============================================================================
*/

::OSStatus  ThreadBaseImpl::dispatch_and_run (void * parameter)
{
   ThreadBaseImpl *  this_ptr =
      reinterpret_cast <ThreadBaseImpl *> (parameter);

   try
   {
      ThreadBase * thread_ptr = this_ptr->_thread_ptr;
      thread_ptr->do_run ();
      
      MPSemaphoreID wait_id = this_ptr->_wait_id;
      
      OSStatus err = MPSignalSemaphore (wait_id);
      assert (err == 0);
   }
   catch (...)
   {
      ohm_util_TRACE_PANIC ("ThreadBaseImpl::dispatch_and_run : unhandled exception");
      
      assert (false);
   }

   return (::noErr);
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  ThreadBaseImpl::init ()
{
   OSStatus err = MPCreateBinarySemaphore (&_wait_id);
   assert (err == 0);
   
   err = MPWaitOnSemaphore (_wait_id, kDurationImmediate);
   assert (err == 0);
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  ThreadBaseImpl::restore ()
{
   if (_wait_id != 0)
   {
      OSStatus err = MPDeleteSemaphore (_wait_id);
      assert (err == 0);
      _wait_id = 0;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
