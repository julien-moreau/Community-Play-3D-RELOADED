/*****************************************************************************

        ThreadMgr.cpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 65619

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

#include "ohm/sys/ThreadMgr.h"
#include "ohm/sys/Err.h"

#include <mach/thread_act.h>
#include <mach/mach_host.h>
#include <mach/host_info.h>
#include <pthread.h>

#include <unistd.h>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : yield
==============================================================================
*/

void  ThreadMgr::yield ()
{
#if (ohm_archi_ARCH == ohm_archi_ARCH_MACHO)
   YieldToAnyThread ();
#endif
}



/*
==============================================================================
Name : sleep
==============================================================================
*/

void  ThreadMgr::sleep (double time_s)
{
   ::usleep ((useconds_t) (1000000.0 * time_s)); 
}



/*
==============================================================================
Name : get_current_thread_id
==============================================================================
*/

ThreadMgr::IdType ThreadMgr::get_current_thread_id ()
{
   return (MPCurrentTaskID ());
}



/*
==============================================================================
Name : get_invalid_thread_id
Description :
   Returns an identifier which cannot be a valid thread id.
   This is used to ctor IdType and check later if it was initialized or not.
==============================================================================
*/

ThreadMgr::IdType ThreadMgr::get_invalid_thread_id ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #error to do

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   return 0;
   
#elif (ohm_archi_SYS == ohm_archi_SYS_BEOS)
   #error to do

#else
   #error Unsupported operating system.

#endif
}



/*
==============================================================================
Name : count_nbr_logical_proc
==============================================================================
*/

int   ThreadMgr::count_nbr_logical_proc ()
{
   host_basic_info_data_t info;
   mach_msg_type_number_t info_cnt = HOST_BASIC_INFO_COUNT;

   kern_return_t result = host_info (
      mach_host_self (),
      HOST_BASIC_INFO, 
      (host_info_t) &info,
      &info_cnt
   );
   assert (result == KERN_SUCCESS);
   
   int nbr_logical_proc = 1;
   
   if (result == KERN_SUCCESS)
   {
      nbr_logical_proc = info.logical_cpu;
   }
   
   return nbr_logical_proc;
}



/*
==============================================================================
Name : set_thread_name
==============================================================================
*/

void  ThreadMgr::set_thread_name (const char *name_0)
{
   // only in 10.6 SDK
   // will work only in debugger for XCode >= 3.2
   //pthread_setname_np (pthread_self (), name_0);
}



/*
==============================================================================
Name : set_priority
==============================================================================
*/

int   ThreadMgr::set_priority (Priority prio)
{
#if 1
   thread_precedence_policy_data_t     precedence_policy;
   
   // it happens that 0 is normal, and importance is ordinal
   precedence_policy.importance = int (prio);
   
   kern_return_t result = thread_policy_set (
      mach_thread_self (), 
      THREAD_PRECEDENCE_POLICY,  
      (thread_policy_t) &precedence_policy, 
      THREAD_PRECEDENCE_POLICY_COUNT
   );
   assert (result == KERN_SUCCESS);
   
   int err = 0;
   
   if (result != KERN_SUCCESS)
   {
      err = Err_CANNOT_SET_PRIORITY;
   }
   
   return err;

#elif 0
   int err = 0;
         
   if (prio == Priority_HIGH)
   {
      if (err == 0)
      {
         thread_time_constraint_policy_data_t policy;
         
         policy.period = 0;
         policy.computation = 5000000;
         policy.constraint = 10000000;
         policy.preemptible = true;
         
         kern_return_t result = thread_policy_set (
            mach_thread_self (), 
            THREAD_TIME_CONSTRAINT_POLICY,  
            (thread_policy_t) &policy, 
            THREAD_TIME_CONSTRAINT_POLICY_COUNT
         );
         assert (result == KERN_SUCCESS);
         
         if (result != KERN_SUCCESS)
         {
            err = Err_CANNOT_SET_PRIORITY;
         }
      }

      if (err == 0)
      {
         thread_precedence_policy_data_t policy;
         
         policy.importance = 1;
         
         kern_return_t result = thread_policy_set (
            mach_thread_self (), 
            THREAD_PRECEDENCE_POLICY,  
            (thread_policy_t) &policy, 
            THREAD_PRECEDENCE_POLICY_COUNT
         );
         assert (result == KERN_SUCCESS);
         
         if (result != KERN_SUCCESS)
         {
            err = Err_CANNOT_SET_PRIORITY;
         }
      }

      if (err == 0)
      {
         thread_extended_policy policy;
         
         policy.timeshare = false;
         
         kern_return_t result = thread_policy_set (
            mach_thread_self (), 
            THREAD_EXTENDED_POLICY,  
            (thread_policy_t) &policy, 
            THREAD_EXTENDED_POLICY_COUNT
         );
         assert (result == KERN_SUCCESS);
         
         if (result != KERN_SUCCESS)
         {
            err = Err_CANNOT_SET_PRIORITY;
         }
      }
   }
   
   return err;

#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
