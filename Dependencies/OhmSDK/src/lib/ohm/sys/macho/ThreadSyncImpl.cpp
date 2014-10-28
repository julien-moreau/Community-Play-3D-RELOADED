/*****************************************************************************

        ThreadSyncImpl.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64824

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

#include "ohm/sys/macho/ThreadSyncImpl.h"

#include <stdexcept>

#include <sys/time.h>
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

ThreadSyncImpl::ThreadSyncImpl ()
:  _cond ()
,  _mutex ()
,  _condition_cnt (0)
{
   pthread_cond_init (&_cond, 0);
   pthread_mutex_init (&_mutex, 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ThreadSyncImpl::~ThreadSyncImpl ()
{
   pthread_cond_destroy (&_cond);
   pthread_mutex_destroy (&_mutex);
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  ThreadSyncImpl::signal (int cnt)
{
   pthread_mutex_lock (&_mutex);
   
   _condition_cnt += cnt;
   
   // on Windows there is no guaranted way to
   // wake up all threads waiting on the condition
   // so we are replicating the same behavior here
   // (i.e. don't use pthread_cond_broadcast)
   
   pthread_cond_signal (&_cond);
   
   pthread_mutex_unlock (&_mutex);
}



/*
==============================================================================
Name : wait
==============================================================================
*/

bool  ThreadSyncImpl::wait (double timeout)
{
   pthread_mutex_lock (&_mutex);

   bool           timeout_flag = false;
   if (timeout >= 0)
   {
      using namespace std;

      timespec ts;
      ts.tv_sec = static_cast <time_t> (floor (timeout));
      ts.tv_nsec = static_cast <long> ((timeout - floor (timeout)) * 1.0e9);
      
      while ((_condition_cnt == 0) && ! timeout_flag)
      {
         int err = pthread_cond_timedwait_relative_np (&_cond, &_mutex, &ts);
         assert ((err == 0) || (err == ETIMEDOUT));
         
         if (err == ETIMEDOUT)
         {
            timeout_flag = true;
         }
      }
   }

   else
   {
      while (_condition_cnt == 0)
      {
         int err = pthread_cond_wait (&_cond, &_mutex);
         assert (err == 0);
      }
   }
   
   if (!timeout_flag)
   {
      --_condition_cnt;
   }
   
   pthread_mutex_unlock (&_mutex);

   return (timeout_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
