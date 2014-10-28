/*****************************************************************************

        Date.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68319

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



#if ! defined (ohm_util_Date_HEADER_INCLUDED)
#define  ohm_util_Date_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/sys/TimeZoneType.h"



namespace ohm
{

namespace sys
{
   class UtcTime;
}

namespace util
{



class BinaryStreamInput;
class BinaryStreamOutputBase;

class Date
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         PRINT_LEN_MAX        = 24  }; // Without trailing '\0'
   enum {         YEAR_MIN             = -10000 };
   enum {         YEAR_MAX             = +10000 };
   enum {         NBR_MONTHS_PER_YEAR  = 12  };
   enum {         NBR_DAYS_PER_WEEK    = 7   };

                  Date ();
                  Date (int year, int month, int day, int hour = 0, int minute = 0, double second = 0);
                  Date (const char *date_0);
                  Date (const sys::UtcTime &utc_time, sys::TimeZoneType zone);
                  Date (const Date &other);
   virtual        ~Date () {}
   
   void           write (BinaryStreamOutputBase & bso) const;
   int            read (BinaryStreamInput & bsi);
   
   void           set (int year, int month, int day, int hour = 0, int minute = 0, double second = 0);
   int            set (const char *date_0);
   int            get_year () const;
   int            get_month () const;
   int            get_day () const;
   int            get_hour () const;
   int            get_minute () const;
   double         get_second () const;

   void           print (char txt_0 [PRINT_LEN_MAX+1]) const;

   double         compute_duration_since (const Date &other) const;

   void           add_time (int year, int month, int day, int hour, int minute, double second);

   static bool    is_bissextile_year (int year);
   static int     get_days_per_month (int year, int month);
   static int     compute_day_of_the_year (int year, int month, int day);
   static long    compute_nbr_days_from_christ (int year, int month, int day);
   static void    make_date_w_nbr_days_from_christ (int &year, int &month, int &day, long nbr_days);

   static const char *
                  MONTH_NAME_0_LIST [NBR_MONTHS_PER_YEAR];
   static const char *
                  DAY_NAME_0_LIST [NBR_DAYS_PER_WEEK];



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   int            _year;   // Year...
   int            _month;  // [0 ; 11]
   int            _day;    // [0 ; 30]
   int            _hour;   // [0 ; 23]
   int            _minute; // [0 ; 59]
   double         _second; // [0 ; 60[

   static const int
                  DAYS_PER_MONTH [NBR_MONTHS_PER_YEAR];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           operator == (const Date &other);
   bool           operator != (const Date &other);

}; // class Date



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_util_Date_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
