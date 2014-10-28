/*****************************************************************************

        ThreadBase.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68831

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

#include "ohm/lang/def.h"
#include "ohm/math/fnc.h"
#include "ohm/sys/win32/ThreadBaseImpl.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <windows.h>
#if (ohm_sys_ThreadBase_CRT == ohm_sys_ThreadBase_CRT_USE)
#endif   // ohm_sys_ThreadBase_CRT
#include <process.h>

#include <cassert>



namespace ohm
{
namespace sys
{



// To dump handle in traces
ohm_lang_CHECK_CST (
   HandleSize,
   sizeof (::HANDLE) <= sizeof (ptrdiff_t)
);



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ThreadBase::ThreadBase ()
:  _impl_ptr (new ThreadBaseImpl)
{
   assert (_impl_ptr != 0);

#if (ohm_sys_ThreadBase_CRT == ohm_sys_ThreadBase_CRT_USE)

   _impl_ptr->_handle = reinterpret_cast <::HANDLE> (::_beginthreadex (
      0,
      0,
      ThreadBaseImpl::dispatch_and_run,
      reinterpret_cast <::LPVOID> (this),
      CREATE_SUSPENDED,
      &_impl_ptr->_id
   ));

#else // ohm_sys_ThreadBase_CRT

   _impl_ptr->_handle = ::CreateThread (
      0,
      0,
      ThreadBaseImpl::dispatch_and_run,
      reinterpret_cast <::LPVOID> (this),
      CREATE_SUSPENDED,
      &_impl_ptr->_id
   );

#endif   // ohm_sys_ThreadBase_CRT

   if (_impl_ptr->_handle == 0)
   {
      ohm_util_TRACE_ERROR ("ctor(): Cannot create thread.");
      throw std::runtime_error (
         "ohm::sys::ThreadBase::ThreadBase(): "
         "Cannot create thread."
      );
   }
}



ThreadBase::~ThreadBase ()
{
   if (_impl_ptr->_run_flag)
   {
      ohm_util_TRACE_WARNING ("dtor(): Thread still running, kill required.");
      kill ();
   }

   if (::CloseHandle (_impl_ptr->_handle) == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "dtor(): Failed to close thread handle 0x%1X%. "
         "Returned Windows error code 0x%2rm8w0X%.",
         reinterpret_cast <ptrdiff_t> (_impl_ptr->_handle),
         err_code
      );
   }
   _impl_ptr->_handle = 0;

   delete _impl_ptr;
   _impl_ptr = 0;
}



int   ThreadBase::run ()
{
   assert (_impl_ptr != 0);
   assert (_impl_ptr->_handle != 0);
   assert (! _impl_ptr->_term_flag);

   int            ret_val = Err_OK;

   if (::ResumeThread (_impl_ptr->_handle) == 0xFFFFFFFFUL)
   {
      ret_val = Err_CANNOT_START_THREAD;
   }

   return (ret_val);
}



// timeout is measured in seconds, and negative if no timeout required.
// Returns true if the thread is dead (no timeout).
bool  ThreadBase::wait_for_death (double timeout)
{
   assert (_impl_ptr != 0);
   assert (_impl_ptr->_handle != 0);
   assert (timeout < 1000);

   bool           dead_flag = false;
   if (_impl_ptr->_term_flag)
   {
      dead_flag = true;
   }

   else
   {
      ::DWORD        timeout_ms = INFINITE;
      if (timeout >= 0)
      {
         timeout_ms = math::round_int (timeout * 1000);
      }

      const ::DWORD  result =
         ::WaitForSingleObject (_impl_ptr->_handle, timeout_ms);
      if (result == WAIT_FAILED)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR2 (
            "wait_for_death(): Failed for thread 0x%1X%. "
            "Returned Windows error code 0x%2rm8w0X%.",
            reinterpret_cast <ptrdiff_t> (_impl_ptr->_handle),
            err_code
         );
      }
      else if (result == WAIT_TIMEOUT)
      {
         ohm_util_TRACE_WARNING1 (
            "wait_for_death(): timeout for thread 0x%1X%.",
            reinterpret_cast <ptrdiff_t> (_impl_ptr->_handle)
         );
      }
      else
      {
         dead_flag = true;
         _impl_ptr->_term_flag = true;
      }
   }

   return (dead_flag);
}



// Use only in case of emergency (kills brutally, doesn't deallocate anything).
void  ThreadBase::kill ()
{
   const ::BOOL      kill_res = ::TerminateThread (_impl_ptr->_handle, -1);
   const ::DWORD     cur_thread_id = ::GetCurrentThreadId ();
   if (kill_res == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR3 (
         "kill(): thread 0x%3X% failed to murder thread 0x%1X%. "
         "Returned Windows error code 0x%2rm8w0X%.",
         reinterpret_cast <ptrdiff_t> (_impl_ptr->_handle),
         err_code,
         cur_thread_id
      );
   }
   else
   {
      ohm_util_TRACE_INFO2 (
         "kill(): thread 0x%2X% murdered thread 0x%1X%.",
         reinterpret_cast <ptrdiff_t> (_impl_ptr->_handle),
         cur_thread_id
      );
   }
   
   _impl_ptr->_run_flag = false;
   _impl_ptr->_term_flag = true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
