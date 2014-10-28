/*****************************************************************************

        HttpStats.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71330

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

#include "plex/http/HttpStats.h"

#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/ascii/ascii_fnc.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_start_date
==============================================================================
*/

std::string HttpStats::get_start_date ()
{
   tm tm_start_time;
   gmtime_r (&_stats_data._start_time, &tm_start_time);
   
   char date_0 [100];
   strftime (date_0, sizeof (date_0), "%a, %d %b %Y %H:%M:%S GMT", &tm_start_time);
   
   return std::string (date_0);
}



/*
==============================================================================
Name : get_running_time
==============================================================================
*/

std::string HttpStats::get_running_time ()
{
   time_t now = time (0);
   
   double diff_s = difftime (now, _stats_data._start_time);
   diff_s = floor (diff_s);
   
   double day_s = 60.0 * 60.0 * 24.0;
   int nbr_day = int (floor (diff_s / day_s));
   diff_s -= double (nbr_day) * day_s;

   double hour_s = 60.0 * 60.0;
   int nbr_hour = int (floor (diff_s / hour_s));
   diff_s -= double (nbr_hour) * hour_s;

   double minute_s = 60.0;
   int nbr_minute = int (floor (diff_s / minute_s));
   diff_s -= double (nbr_minute) * minute_s;
   
   int nbr_second = int (diff_s);
   
   char diff_0 [100];
   snprintf (diff_0, 100, "%d Days %d Hours %d Minutes %d Seconds", nbr_day, nbr_hour, nbr_minute, nbr_second);
   
   return std::string (diff_0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : push_metric
==============================================================================
*/

HttpStats::HttpStats ()
:  _stats_data ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

