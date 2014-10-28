/*****************************************************************************

        MacTimeFnc.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60003

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

#include "ohm/sys/macho/MacTimeFnc.h"

#include "ohm/archi/types.h"
#include "ohm/sys/UtcTime.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

::CFGregorianDate MacTimeFnc_get_origin_gregorian_date ()
{
   CFGregorianDate origin_date;
   origin_date.year = 1601;
   origin_date.month = 0;
   origin_date.day = 0;
   origin_date.hour = 0;
   origin_date.minute = 0;
   origin_date.second = 0;
   
   return origin_date;
}



::CFAbsoluteTime MacTimeFnc_get_cf_absolute_time_offset ()
{
   static CFGregorianDate origin_date = MacTimeFnc_get_origin_gregorian_date ();
   
   static CFAbsoluteTime time_offset = ::CFGregorianDateGetAbsoluteTime (origin_date, 0); 
   
   return -time_offset;
}



/*
==============================================================================
Name: CFAbsoluteTime_to_UtcTime
==============================================================================
*/

void MacTimeFnc::CFAbsoluteTime_to_UtcTime (UtcTime & utc_time, const ::CFAbsoluteTime & absolute_time)
{
   ::CFAbsoluteTime time = MacTimeFnc_get_cf_absolute_time_offset () + absolute_time;
   
   archi::Int64 ticks = archi::Int64 (time * sys::UtcTime::_ticks_per_sec);
   
   utc_time.set (ticks);
}



/*
==============================================================================
Name: UtcTime_to_CFAbsoluteTime
==============================================================================
*/

void MacTimeFnc::UtcTime_to_CFAbsoluteTime (::CFAbsoluteTime & absolute_time, const UtcTime & utc_time)
{
   const double factor = 1.0 / double (sys::UtcTime::_ticks_per_sec);
   
   absolute_time = double (utc_time.get ()) * factor;
   absolute_time -= MacTimeFnc_get_cf_absolute_time_offset ();
}


void  MacTimeFnc::UTCDateTime_to_UtcTime (UtcTime & utc_time, const ::UTCDateTime & utc_date_time)
{
   ::CFAbsoluteTime absolute_time;

   OSStatus err = ::UCConvertUTCDateTimeToCFAbsoluteTime (&utc_date_time, &absolute_time);
   
   if (err != 0)
   {
      switch (err)
      {
      case kUTCUnderflowErr:
         ohm_util_TRACE_ERROR (
            "UCConvertUTCDateTimeToCFAbsoluteTime: Underflow error"
         );
         break;

      case kUTCOverflowErr:
         ohm_util_TRACE_ERROR (
            "UCConvertUTCDateTimeToCFAbsoluteTime: Overflow error"
         );
         break;

      case kIllegalClockValueErr:
         ohm_util_TRACE_ERROR (
            "UCConvertUTCDateTimeToCFAbsoluteTime: Illegal clock value"
         );
         break;

      default:
         ohm_util_TRACE_ERROR1 (
            "UCConvertUTCDateTimeToCFAbsoluteTime: Unknown error %1%", err
         );
         break;
      }
      
      assert (false);
   }
   
   CFAbsoluteTime_to_UtcTime (utc_time, absolute_time);
}



void  MacTimeFnc::UtcTime_to_UTCDateTime (::UTCDateTime & utc_date_time, const UtcTime & utc_time)
{
   CFAbsoluteTime absolute_time;
   UtcTime_to_CFAbsoluteTime (absolute_time, utc_time);
   
   OSStatus err = ::UCConvertCFAbsoluteTimeToUTCDateTime (absolute_time, &utc_date_time);

   if (err != 0)
   {
      ohm_util_TRACE_ERROR1 ("UCConvertUTCDateTimeToCFAbsoluteTime failed with error %1%", err);
      assert (false);
   }
}


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
