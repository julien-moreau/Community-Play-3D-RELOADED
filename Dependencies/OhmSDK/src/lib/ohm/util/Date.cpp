/*****************************************************************************

        Date.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68437

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
#include "ohm/sys/UtcTime.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"
#include "ohm/util/Date.h"
#include "ohm/util/Err.h"
#include "ohm/util/trace.h"

#include <cassert>
#include <cstdlib>
#include <stdexcept>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



Date::Date ()
:  _year (2000)
,  _month (0)
,  _day (0)
,  _hour (0)
,  _minute (0)
,  _second (0)
{
   // Nothing
}



Date::Date (int year, int month, int day, int hour, int minute, double second)
:  _year (year)
,  _month (month)
,  _day (day)
,  _hour (hour)
,  _minute (minute)
,  _second (second)
{
   assert (year >= YEAR_MIN);
   assert (year <= YEAR_MAX);
   assert (month >= 0);
   assert (month < NBR_MONTHS_PER_YEAR);
   assert (day >= 0);
   assert (day < get_days_per_month (year, month));
   assert (hour >= 0);
   assert (hour < 24);
   assert (minute >= 0);
   assert (minute < 60);
   assert (second >= 0);
   assert (second < 60);
}



// String must be at least 24 char long
// Example: "Thu Sep 04 08:36:09 2003"
//           0   4   8  11 14 17 20
Date::Date (const char *date_0)
{
   assert (date_0 != 0);

   int            ret_val = set (date_0);
   if (ret_val != Err_OK)
   {
      throw std::runtime_error (
         "ohm::sys::Date::Date(): Invalid string."
      );
   }
}



Date::Date (const sys::UtcTime &utc_time, sys::TimeZoneType zone)
:  _year (2000)
,  _month (0)
,  _day (0)
,  _hour (0)
,  _minute (0)
,  _second (0)
{
   assert (&utc_time != 0);
   assert (utc_time.is_valid ());
   assert (zone >= 0);
   assert (zone < sys::TimeZoneType_NBR_ELT);

   utc_time.conv_to_date (*this, zone);
}



Date::Date (const Date &other)
:  _year (other._year)
,  _month (other._month)
,  _day (other._day)
,  _hour (other._hour)
,  _minute (other._minute)
,  _second (other._second)
{
   // Nothing
}



void  Date::write (BinaryStreamOutputBase & bso) const
{
   bso.write (archi::Int32 (_year));
   bso.write (archi::UByte (_month));
   bso.write (archi::UByte (_day));
   bso.write (archi::UByte (_hour));
   bso.write (archi::UByte (_minute));
   bso.write (archi::Float64 (_second));
}



int   Date::read (BinaryStreamInput & bsi)
{
   int err = 0;
   
   if (err == 0)
   {
      err = bsi.read_int32 (_year, "Date: Error while reading _year");
   }
   
   if (err == 0)
   {
      err = bsi.read_ubyte (_month, "Date: Error while reading _month");
   }
   
   if (err == 0)
   {
      err = bsi.read_ubyte (_day, "Date: Error while reading _day");
   }
   
   if (err == 0)
   {
      err = bsi.read_ubyte (_hour, "Date: Error while reading _hour");
   }
   
   if (err == 0)
   {
      err = bsi.read_ubyte (_minute, "Date: Error while reading _minute");
   }
   
   if (err == 0)
   {
      err = bsi.read_float64 (_second, "Date: Error while reading _second");
   }
   
   return err;
}



void  Date::set (int year, int month, int day, int hour, int minute, double second)
{
   assert (year >= YEAR_MIN);
   assert (year <= YEAR_MAX);
   assert (month >= 0);
   assert (month < NBR_MONTHS_PER_YEAR);
   assert (day >= 0);
   assert (day < get_days_per_month (year, month));
   assert (hour >= 0);
   assert (hour < 24);
   assert (minute >= 0);
   assert (minute < 60);
   assert (second >= 0);
   assert (second < 60);

   _year = year;
   _month = month;
   _day = day;
   _hour = hour;
   _minute = minute;
   _second = second;
}



int   Date::set (const char *date_0)
{
   assert (date_0 != 0);

   int            ret_val = Err_OK;

   // Year
   _year =   (date_0 [20] - '0') * 1000
           + (date_0 [21] - '0') * 100
           + (date_0 [22] - '0') * 10
           + (date_0 [23] - '0');
   if (_year < YEAR_MIN || _year > YEAR_MAX)
   {
      ohm_util_TRACE_ERROR ("Invalid year");
      ret_val = Err_INVALID_DATE_STRING;
   }

   // Month
   if (ret_val == Err_OK)
   {
      char           cur_month_0 [4] =
      {
         date_0 [4], date_0 [5], date_0 [6], '\0'
      };
      bool           month_found_flag = false;
      for (int month = 0
      ;  month < NBR_MONTHS_PER_YEAR && ! month_found_flag
      ;  ++month)
      {
         if (strcmp (cur_month_0, MONTH_NAME_0_LIST [month]) == 0)
         {
            month_found_flag = true;
            _month = month;
         }
      }
      if (! month_found_flag)
      {
         ohm_util_TRACE_ERROR ("ctor(): Invalid month");
         ret_val = Err_INVALID_DATE_STRING;
      }
   }

   // Day
   if (ret_val == Err_OK)
   {
      _day = (date_0 [8] - '0') * 10 + (date_0 [9] - '0') - 1;
      if (_day < 0 || _day >= get_days_per_month (_year, _month))
      {
         ohm_util_TRACE_ERROR ("ctor(): Invalid day");
         ret_val = Err_INVALID_DATE_STRING;
      }
   }

   // Hour
   if (ret_val == Err_OK)
   {
      _hour = (date_0 [11] - '0') * 10 + (date_0 [12] - '0');
      if (_hour < 0 || _hour > 23)
      {
         ohm_util_TRACE_ERROR ("ctor(): Invalid hour");
         ret_val = Err_INVALID_DATE_STRING;
      }
   }

   // Minute
   if (ret_val == Err_OK)
   {
      _minute = (date_0 [14] - '0') * 10 + (date_0 [15] - '0');
      if (_minute < 0 || _minute > 59)
      {
         ohm_util_TRACE_ERROR ("ctor(): Invalid minute");
         ret_val = Err_INVALID_DATE_STRING;
      }
   }

   // Second
   if (ret_val == Err_OK)
   {
      _second = (date_0 [17] - '0') * 10 + (date_0 [18] - '0');
      if (_second < 0 || _second > 59)
      {
         ohm_util_TRACE_ERROR ("ctor(): Invalid second");
         ret_val = Err_INVALID_DATE_STRING;
      }
   }

   return (ret_val);
}



int   Date::get_year () const
{
   return (_year);
}



int   Date::get_month () const
{
   return (_month);
}



int   Date::get_day () const
{
   return (_day);
}



int   Date::get_hour () const
{
   return (_hour);
}



int   Date::get_minute () const
{
   return (_minute);
}



double   Date::get_second () const
{
   return (_second);
}



void  Date::print (char txt_0 [PRINT_LEN_MAX+1]) const
{
   assert (txt_0 != 0);

   const long     abs_day = compute_nbr_days_from_christ (_year, _month, _day);
   const int      dotw_index = (abs_day + 5) % NBR_DAYS_PER_WEEK;
   const char *   dotw_name_0 = DAY_NAME_0_LIST [dotw_index];
   txt_0 [0] = dotw_name_0 [0];
   txt_0 [1] = dotw_name_0 [1];
   txt_0 [2] = dotw_name_0 [2];
   txt_0 [3] = ' ';

   const char *   month_name_0 = MONTH_NAME_0_LIST [_month];
   txt_0 [4] = month_name_0 [0];
   txt_0 [5] = month_name_0 [1];
   txt_0 [6] = month_name_0 [2];
   txt_0 [7] = ' ';

   txt_0 [8] = '0' + (_day + 1) / 10;
   txt_0 [9] = '0' + (_day + 1) % 10;
   txt_0 [10] = ' ';

   const int      sec_int = math::floor_int (_second);
   txt_0 [11] = '0' + _hour / 10;
   txt_0 [12] = '0' + _hour % 10;
   txt_0 [13] = ':';
   txt_0 [14] = '0' + _minute / 10;
   txt_0 [15] = '0' + _minute % 10;
   txt_0 [16] = ':';
   txt_0 [17] = '0' + sec_int / 10;
   txt_0 [18] = '0' + sec_int % 10;
   txt_0 [19] = ' ';

   txt_0 [20] = '0' + (_year / 1000) % 10;
   txt_0 [21] = '0' + (_year / 100) % 10;
   txt_0 [22] = '0' + (_year / 10) % 10;
   txt_0 [23] = '0' + _year % 10;

   txt_0 [24] = '\0';
   assert (24 == PRINT_LEN_MAX);
}



// Computes this - other in seconds
double   Date::compute_duration_since (const Date &other) const
{
   assert (&other != 0);

   // Days relative to 0
   const long     day_0_l = compute_nbr_days_from_christ (
      _year,
      _month,
      _day
   );
   const long     day_0_r = compute_nbr_days_from_christ (
      other._year,
      other._month,
      other._day
   );
   const long     day_0_dif = day_0_l - day_0_r;
   double         nbr_sec = static_cast <double> (day_0_dif) * (3600.0 * 24);

   // Seconds relative to days
   const long     diff_h = (_hour - other._hour) * 3600L;
   const long     diff_m = (_minute - other._minute) * 60L;
   const double   diff_s = _second - other._second;
   nbr_sec += static_cast <double> (diff_h);
   nbr_sec += static_cast <double> (diff_m);
   nbr_sec += diff_s;

   return (nbr_sec);
}



void  Date::add_time (int year, int month, int day, int hour, int minute, double second)
{
   // Adds time
   _hour += hour;
   _minute += minute;
   _second += second;

   // Fix time and reports excessive time on day offset
   if (_second < 0 || _second >= 60)
   {
      const int   m = math::floor_int (_second / 60.0);
      _second -= m * 60;
      _minute += m;
   }
   if (_minute < 0 || _minute >= 60)
   {
      const int   h = math::floor_int (_minute / 60.0);
      _minute -= h * 60;
      _hour += h;
   }
   if (_hour < 0 || _hour >= 24)
   {
      const int   d = math::floor_int (_hour / 24.0);
      _hour -= d * 24;
      day += d;
   }

   // Adds months and years
   _year += year;
   _month += month;

   // Fix months
   if (_month < 0 || _month >= NBR_MONTHS_PER_YEAR)
   {
      const int   y = math::floor_int (
         static_cast <double> (_month) / NBR_MONTHS_PER_YEAR
      );
      _month -= y * NBR_MONTHS_PER_YEAR;
      _year += y;
   }

   // Now converts resulting year/month into absolute days to add days
   long           day_abs = compute_nbr_days_from_christ (_year, _month, 0);
   day_abs += _day + day;
   make_date_w_nbr_days_from_christ (_year, _month, _day, day_abs);
}



bool  Date::is_bissextile_year (int year)
{
   assert (year >= YEAR_MIN);
   assert (year <= YEAR_MAX);
   bool           bis_flag = false;

   bis_flag = ((year & 3) == 0);
   if ((year % 100) == 0)
   {
      assert (bis_flag);
      bis_flag = ((year % 400) == 0);
   }

   return (bis_flag);
}



int   Date::get_days_per_month (int year, int month)
{
   assert (year >= YEAR_MIN);
   assert (year <= YEAR_MAX);
   assert (month >= 0);
   assert (month < NBR_MONTHS_PER_YEAR);

   int            days = DAYS_PER_MONTH [month];
   if (   month == 1
       && is_bissextile_year (year))
   {
      days = 29;
   }

   return (days);
}



int   Date::compute_day_of_the_year (int year, int month, int day)
{
   assert (year >= YEAR_MIN);
   assert (year <= YEAR_MAX);
   assert (month >= 0);
   assert (month < NBR_MONTHS_PER_YEAR);
   assert (day >= 0 && day < get_days_per_month (year, month));

   int            day_y = day;
   for (int m = 0; m < month; ++m)
   {
      day_y += get_days_per_month (year, m);
   }

   return (day_y);
}



long  Date::compute_nbr_days_from_christ (int year, int month, int day)
{
   assert (year >= YEAR_MIN);
   assert (year <= YEAR_MAX);
   assert (month >= 0);
   assert (month < 12);
   assert (day >= 0 && day < get_days_per_month (year, month));

   const int      nbr_400 = math::ceil_int (year / 400.0);
   const int      nbr_100 = math::ceil_int (year / 100.0);
   const int      nbr_4 = math::ceil_int (year / 4.0);
   const int      nbr_bis = nbr_4 - nbr_100 + nbr_400;

   const int      day_y = compute_day_of_the_year (year, month, day);

   const long     nbr_days = year * 365L + nbr_bis + day_y;

   return (nbr_days);
}



void  Date::make_date_w_nbr_days_from_christ (int &year, int &month, int &day, long nbr_days)
{
   assert (&year != 0);
   assert (&month != 0);
   assert (&day != 0);

   const long     days_in_1 = 365;
   const long     days_in_4 = days_in_1 * (4/1) + 1;
   const long     days_in_100 = days_in_4 * (100/4) - 1;
   const long     days_in_400 = days_in_100 * (400/100) + 1;

   const int      nbr_400 =
      math::floor_int (static_cast <double> (nbr_days) / days_in_400);
   nbr_days -= nbr_400 * days_in_400;

   // First 100-year period of a 400-year period has 1 day more
   const int      nbr_100 = (nbr_days - 1) / days_in_100;
   const int      fix_100 = (nbr_100 > 0) ? 1 : 0;
   nbr_days -= nbr_100 * days_in_100 + fix_100;

   // First 4-year period of a 100-year period may or may not have 1 day less
   const int      nbr_4 = (nbr_days + fix_100) / days_in_4;
   const int      fix_4 = (nbr_4 > 0 && nbr_100 > 0) ? 1 : 0;
   nbr_days -= nbr_4 * days_in_4 - fix_4;

   // First year may or may not have 1 day more
   const int      offset_1 = (nbr_100 > 0 && nbr_4 == 0) ? 0 : 1; // 1 for periods containing a bissextile year
   const int      nbr_1 = (nbr_days - offset_1) / days_in_1;
   const int      fix_1 = ((nbr_100 == 0 || nbr_4 > 0) && nbr_1 > 0) ? 1 : 0;
   nbr_days -= nbr_1 * days_in_1 + fix_1;

   year = nbr_400 * 400 + nbr_100 * 100 + nbr_4 * 4 + nbr_1;
   assert (nbr_days >= 0);
   assert (nbr_days < (is_bissextile_year (year) ? 366 : 365));
   month = -1;
   do
   {
      ++ month;
      day = nbr_days;
      nbr_days -= get_days_per_month (year, month);
   }
   while (nbr_days >= 0);
}



const char *   Date::MONTH_NAME_0_LIST [NBR_MONTHS_PER_YEAR] =
{
   "Jan", "Feb", "Mar", "Apr", "May", "Jun",
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const char *   Date::DAY_NAME_0_LIST [NBR_DAYS_PER_WEEK] =
{
   "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const int   Date::DAYS_PER_MONTH [NBR_MONTHS_PER_YEAR] =
{
   31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
