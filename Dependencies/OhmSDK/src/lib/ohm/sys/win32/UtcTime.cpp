/*****************************************************************************

        UtcTime.cpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 20718

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
#include "ohm/sys/win32/FileUtilWin32.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/UtcTime.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/Date.h"
#include "ohm/util/trace.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <windows.h>

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Because maximum SYSTEMTIME precision is millisecond, resulting date
// may lose accuracy during the converison.
void  UtcTime::conv_to_date (util::Date &date, TimeZoneType zone) const
{
   assert (is_valid ());
   assert (&date != 0);
   assert (zone >= 0);
   assert (zone < TimeZoneType_NBR_ELT);

   // Converts internal ticks to UTC FILETIME
   ::FILETIME     date_file;
   FileUtilWin32::convert_utc_time_to_file (date_file, *this);

   // If requested date is local, translates the UTC FILETIME to its local
   // value.
   if (zone == TimeZoneType_LOCAL)
   {
      ::FILETIME     tmp;
      if (::FileTimeToLocalFileTime (&date_file, &tmp) == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "conv_to_date(): Cannot convert UTC ticks to local, "
            "Windows error 0x%1rm8w0X%.",
            err_code
         );
         throw std::runtime_error (
            "ohm::sys::UtcTime::conv_to_date(): "
            "Cannot convert UTC ticks to local."
         );
      }
      date_file = tmp;
   }

   // Converts absolute FILETIME to human-readable date.
   ::SYSTEMTIME   date_sys;
   if (::FileTimeToSystemTime (&date_file, &date_sys) == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "conv_to_date(): Cannot convert ticks to human-readable date, "
         "Windows error 0x%1rm8w0X%.",
         err_code
      );
      throw std::runtime_error (
         "ohm::sys::UtcTime::conv_to_date(): "
         "Cannot convert ticks to human-readable date."
      );
   }

   const double   ms = static_cast <double> (date_sys.wMilliseconds) / 1000.0;
   date.set (
      date_sys.wYear,
      date_sys.wMonth - 1,
      date_sys.wDay - 1,
      date_sys.wHour,
      date_sys.wMinute,
      static_cast <double> (date_sys.wSecond) + ms
   );
}



// Because maximum SYSTEMTIME precision is millisecond, resulting date
// may lose accuracy during the converison.
int   UtcTime::conv_from_date (const util::Date &date, TimeZoneType zone)
{
   assert (&date != 0);
   assert (zone >= 0);
   assert (zone < TimeZoneType_NBR_ELT);

   int            ret_val = Err_OK;

   // Converts human-readable date to absolute FILETIME.
   const double   round_cst = 0.5 / static_cast <double> (_ticks_per_sec);
   const double   s_dbl = date.get_second () + round_cst;
   const int      s_int = math::floor_int (s_dbl);
   const int      ms = math::floor_int ((s_dbl - s_int) * 1000);
   assert (ms >= 0);
   assert (ms < 1000);

   ::SYSTEMTIME   date_sys;
   date_sys.wYear         = date.get_year ();
   date_sys.wMonth        = date.get_month () + 1;
   date_sys.wDayOfWeek    = 0;   // Ignored
   date_sys.wDay          = date.get_day () + 1;
   date_sys.wHour         = date.get_hour ();
   date_sys.wMinute       = date.get_minute ();
   date_sys.wSecond       = s_int;
   date_sys.wMilliseconds = ms;

   ::FILETIME     date_file;
   if (::SystemTimeToFileTime (&date_sys, &date_file) == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "conv_from_date(): Cannot convert human-readable date to ticks, "
         "Windows error code 0x%1rm8w0X%.",
         err_code
      );

      ret_val = Err_CANNOT_CONVERT_LOCAL_DATE_TO_UTC;
   }

   // If provided date is local, translates the local value to UTC FILETIME.
   if (ret_val == Err_OK)
   {
      if (zone == TimeZoneType_LOCAL)
      {
         ::FILETIME     tmp;
         if (::LocalFileTimeToFileTime (&date_file, &tmp) == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "conv_to_date(): Cannot convert local ticks to UTC, "
               "Windows error 0x%1rm8w0X%.",
               err_code
            );

            ret_val = Err_CANNOT_CONVERT_DATE_TO_TICKS;
         }
         else
         {
            date_file = tmp;
         }
      }
   }

   // Converts UTC FILETIME to internal ticks
   if (ret_val == Err_OK)
   {
      FileUtilWin32::convert_utc_time_from_file (*this, date_file);
   }

   return (ret_val);
}



void  UtcTime::get_current_time (UtcTime &t)
{
   assert (&t != 0);

   ::SYSTEMTIME   date_sys;
   ::GetSystemTime (&date_sys);

   ::FILETIME     date_file;
   if (::SystemTimeToFileTime (&date_sys, &date_file) == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "get_current_time(): Cannot convert human-readable date to ticks, "
         "Windows error code 0x%1rm8w0X%.",
         err_code
      );
      throw std::runtime_error (
         "ohm::sys::UtcTime::get_current_time(): "
         "Cannot convert human-readable date to ticks."
      );
   }

   // Converts UTC FILETIME to internal ticks
   FileUtilWin32::convert_utc_time_from_file (t, date_file);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
