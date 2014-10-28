/*****************************************************************************

        ThreadMgr.cpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 69629

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

#include "ohm/math/fnc.h"
#include "ohm/sys/ConvString.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <windows.h>

#include <cassert>



namespace ohm
{
namespace sys
{



static void ThreadMgr_set_thread_name_internal (::DWORD size, const ::ULONG_PTR *info_ptr)
{
   __try
   {
      ::RaiseException (0x406D1388, 0, size, info_ptr);
   }
   __except (EXCEPTION_CONTINUE_EXECUTION)
   {
      // Nothing
   }
}



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  ThreadMgr::yield ()
{
   ::SwitchToThread ();
}



void  ThreadMgr::sleep (double time_s)
{
   assert (time_s > 0);

   const long     time_ms = ohm::math::ceil_int (time_s * 1000);
   :: Sleep (time_ms);
}



ThreadMgr::IdType ThreadMgr::get_current_thread_id ()
{
   return (::GetCurrentThreadId ());
}



ThreadMgr::IdType ThreadMgr::get_invalid_thread_id ()
{
   // see http://msdn.microsoft.com/en-us/library/ms686746(VS.85).aspx
   return (0);
}



// See: How to: Set a Thread Name in Native Code
// http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
// name_0 is an UTF-8 string.
void  ThreadMgr::set_thread_name (const char *name_0)
{
   assert (name_0 != 0);

   std::string    name_native;
   ConvString::conv_utf8_string_to_native (name_native, name_0, true);

   struct
   {
      ::DWORD        _type;      // must be 0x1000
      ::LPCSTR       _name;      // pointer to name (in user addr space)
      ::DWORD        _thread_id; // thread ID (-1=caller thread)
      ::DWORD        _flags;     // reserved for future use, must be zero
   }              info =
   {
      0x1000,
      name_native.c_str (),
      -1,
      0
   };

   ThreadMgr_set_thread_name_internal (
      sizeof (info) / sizeof (::ULONG_PTR),
      reinterpret_cast <::ULONG_PTR *> (&info)
   );
}



int   ThreadMgr::count_nbr_logical_proc ()
{
   int            nbr_proc = 0;

   ::DWORD_PTR    mask_proc;
   ::DWORD_PTR    mask_sys;
   const ::HANDLE proc_hnd = ::GetCurrentProcess ();
   const ::BOOL   res =
      ::GetProcessAffinityMask (proc_hnd, &mask_proc, &mask_sys);
   if (res != 0)
   {
      if (mask_proc == 0 && mask_sys == 0)
      {
         nbr_proc = 64;
      }
      else
      {
         while (mask_proc != 0)
         {
            nbr_proc += int (mask_proc & 1);
            mask_proc >>= 1;
         }
      }
   }

   if (nbr_proc <= 0)
   {
      nbr_proc = 1;
   }

   return (nbr_proc);
}



int   ThreadMgr::set_priority (Priority prio)
{
   int            ret_val = Err_OK;

   int            prio_win = THREAD_PRIORITY_NORMAL;
   if (prio <= Priority_LOW)
   {
      prio_win = THREAD_PRIORITY_LOWEST;
   }
   else if (prio >= Priority_HIGH)
   {
      prio_win = THREAD_PRIORITY_TIME_CRITICAL;
   }

   const ::HANDLE thread_hnd = ::GetCurrentThread ();
   ::BOOL         res = ::SetThreadPriority (thread_hnd, prio_win);
   if (res == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "set_priority(): failed on handle 0x%1X%. "
         "Returned Windows error code 0x%2rm8w0X%.",
         reinterpret_cast <ptrdiff_t> (thread_hnd),
         err_code
      );

      ret_val = Err_CANNOT_SET_PRIORITY;
   }

   return (ret_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
