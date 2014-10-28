/*****************************************************************************

        ThreadSyncImpl.cpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 64466

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/win32/ThreadSyncImpl.h"
#include "ohm/math/Approx.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ThreadSyncImpl::ThreadSyncImpl ()
:  _handle (0)
{
   _handle = ::CreateSemaphore (0, 0, 123456789, 0);
   if (_handle == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "ctor(): Cannot create semaphore. "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code
      );
      throw std::runtime_error (
         "ohm::sys::ThreadSyncImpl::ctor(): Cannot create event."
      );
   }
}



ThreadSyncImpl::~ThreadSyncImpl ()
{
   assert (_handle != 0);
   const ::BOOL   result = ::CloseHandle (_handle);
   if (result == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "dtor(): Cannot close handle 0x%1X%. "
         "Returned Windows error code 0x%2rm8w0X%.",
         reinterpret_cast <ptrdiff_t> (_handle),
         err_code
      );
   }
   _handle = 0;
}



void  ThreadSyncImpl::signal (int cnt)
{
   ::LONG         prev_count;
   const ::BOOL   result =
      ::ReleaseSemaphore (_handle, cnt, &prev_count);

   if (result == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "signal(): Cannot release sempaphore (handle 0x%1X%). "
         "Returned Windows error code 0x%2rm8w0X%.",
         reinterpret_cast <ptrdiff_t> (_handle),
         err_code
      );
   }
}



bool  ThreadSyncImpl::wait (double timeout)
{
   ::DWORD        timeout_ms = INFINITE;
   if (timeout >= 0)
   {
      timeout_ms = math::Approx::round_int (timeout * 1000);
   }

   const ::DWORD  result =
      ::WaitForSingleObjectEx (_handle, timeout_ms, TRUE);

   if (   result == WAIT_OBJECT_0
       || result == WAIT_TIMEOUT)
   {
      // nothing
   }
   else if (result == WAIT_FAILED)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "wait(): wait failed on handle 0x%1X%. "
         "Returned Windows error code 0x%2rm8w0X%.",
         reinterpret_cast <ptrdiff_t> (_handle),
         err_code
      );
      throw std::runtime_error (
         "ohm::sys::ThreadSyncImpl::wait(): wait failed."
      );
   }
   else
   {
      ohm_util_TRACE_ERROR2 (
         "wait(): WaitForSingleObjectEx on handle 0x%1X%. "
         "unexpectedly failed (returned 0x%2rm8w0X%).",
         reinterpret_cast <ptrdiff_t> (_handle),
         result
      );
      throw std::runtime_error (
         "ohm::sys::ThreadSyncImpl::wait(): unexpected error."
      );
   }

   return (result == WAIT_TIMEOUT);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
