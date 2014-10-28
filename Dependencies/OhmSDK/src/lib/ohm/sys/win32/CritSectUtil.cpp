/*****************************************************************************

        CritSectUtil.cpp
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 39647

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

#include "ohm/lang/def.h"
#include "ohm/sys/win32/CritSectUtil.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <cassert>
#include <cstdio>
#include <cstring>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const char *   CritSectUtil::init (::CRITICAL_SECTION &crit_sec)
{
   assert (&crit_sec != 0);

#if defined (_MSC_VER)

   bool           ok_flag = false;

   __try
   {
      const DWORD    ret_val = 0;
         ::InitializeCriticalSectionAndSpinCount (&crit_sec, 1000);
      ok_flag = (ret_val != 0);
   }
   __except (EXCEPTION_EXECUTE_HANDLER)
   {
      ok_flag = false;
   }

   const char *   err_string_0 = 0;

   if (! ok_flag)
   {
      __try
      {
         ::InitializeCriticalSection (&crit_sec);
         ok_flag = true;
      }
      __except (EXCEPTION_EXECUTE_HANDLER)
      {
         using namespace std;

         const int      code = static_cast <int> (::GetExceptionCode ());
         const long     max_len = 127;
         static char    txt_0 [max_len+1];
         sprintf (
            txt_0,
            "ohm::sys::Mutex::Mutex_init(): "
            "Exception during InitializeCriticalSection(), code %08X.",
            code
         );
         assert (strlen (txt_0) <= max_len);
         err_string_0 = txt_0;
      }
   }

   assert ((ok_flag ^ (err_string_0 != 0)) != 0);

   return (err_string_0);

#elif defined (__GNUC__)

   bool           ok_flag = false;

   try
   {
      const DWORD    ret_val = 0;
         ::InitializeCriticalSectionAndSpinCount (&crit_sec, 1000);
      ok_flag = (ret_val != 0);
   }
   catch (...)
   {
      ok_flag = false;
   }

   const char *   err_string_0 = 0;

   if (! ok_flag)
   {
      try
      {
         ::InitializeCriticalSection (&crit_sec);
         ok_flag = true;
      }
      catch (...)
      {
         using namespace std;

//       const int      code = static_cast <int> (::GetExceptionCode ());
         const long     max_len = 127;
         static char    txt_0 [max_len+1];
         sprintf (
            txt_0,
            "ohm::sys::Mutex::Mutex_init(): "
            "Exception during InitializeCriticalSection()."
         );
         assert (strlen (txt_0) <= max_len);
         err_string_0 = txt_0;
      }
   }

   assert ((ok_flag ^ (err_string_0 != 0)) != 0);

   return (err_string_0);

#else    // _MSC_VER

#error Compiler not supported

#endif   // _MSC_VER
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
