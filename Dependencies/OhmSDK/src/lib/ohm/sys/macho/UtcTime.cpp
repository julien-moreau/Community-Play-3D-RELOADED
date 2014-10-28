/*****************************************************************************

        UtcTime.cpp
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

#include "ohm/math/fnc.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/UtcTime.h"
#include "ohm/util/Date.h"
#include "ohm/util/trace.h"
#include "ohm/sys/macho/MacTimeFnc.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  UtcTime::conv_to_date (util::Date &date, TimeZoneType zone) const
{
   assert (is_valid ());
   assert (&date != 0);
   assert (zone >= 0);
   assert (zone < TimeZoneType_NBR_ELT);
   
   CFAbsoluteTime absolute_time;
   MacTimeFnc::UtcTime_to_CFAbsoluteTime (absolute_time, *this);  
   
   CFTimeZoneRef time_zone_ref = 0;
   
   if (zone == TimeZoneType_LOCAL)
   {
      time_zone_ref = CFTimeZoneCopySystem ();
   }
   else if (zone == TimeZoneType_UTC)
   {
      // 0 for GMT/UTC
   }
   else
   {
      assert (false);
   }

   CFGregorianDate gregorian_date = CFAbsoluteTimeGetGregorianDate (absolute_time, time_zone_ref);
   
   if (time_zone_ref != 0)
   {
      ::CFRelease (time_zone_ref);
   }

   date.set (
      gregorian_date.year,
      gregorian_date.month - 1,
      gregorian_date.day - 1,
      gregorian_date.hour,
      gregorian_date.minute,
      gregorian_date.second
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
   
   CFGregorianDate gregorian_date;
   gregorian_date.year = date.get_year ();
   gregorian_date.month = date.get_month () + 1;
   gregorian_date.day = date.get_day () + 1;
   gregorian_date.hour = date.get_hour ();
   gregorian_date.minute = date.get_minute ();
   gregorian_date.second = date.get_second ();
   
   bool ok_flag = CFGregorianDateIsValid (gregorian_date, kCFGregorianAllUnits);
   assert (ok_flag);
   
   if (!ok_flag)
   {
      ret_val = Err_CANNOT_CONVERT_LOCAL_DATE_TO_UTC;
   }
   
   if (ret_val == Err_OK)
   {
      CFTimeZoneRef time_zone_ref = 0;
      
      if (zone == TimeZoneType_LOCAL)
      {
         time_zone_ref = CFTimeZoneCopySystem ();
      }
      else if (zone == TimeZoneType_UTC)
      {
         // 0 for UTC/GMT
      }
      else
      {
         assert (false);
      }
      
      CFAbsoluteTime absolute_time = CFGregorianDateGetAbsoluteTime (gregorian_date, time_zone_ref);
      
      MacTimeFnc::CFAbsoluteTime_to_UtcTime (*this, absolute_time);

      if (time_zone_ref != 0)
      {
         ::CFRelease (time_zone_ref);
      }
   }
   
   return (ret_val);
}



void  UtcTime::get_current_time (UtcTime &t)
{
   assert (&t != 0);

   CFAbsoluteTime absolute_time = CFAbsoluteTimeGetCurrent ();

   MacTimeFnc::CFAbsoluteTime_to_UtcTime (t, absolute_time);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
