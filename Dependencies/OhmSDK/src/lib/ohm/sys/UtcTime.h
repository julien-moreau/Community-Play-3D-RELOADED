/*****************************************************************************

        UtcTime.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 17793

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



#if ! defined (ohm_sys_UtcTime_HEADER_INCLUDED)
#define  ohm_sys_UtcTime_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"
#include "ohm/sys/TimeZoneType.h"

#include <ctime>



namespace ohm
{

namespace util
{
   class Date;
}

namespace sys
{



class UtcTime
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  UtcTime ();
   explicit       UtcTime (ohm_archi_STD_NAME (time_t) date_c);
                  UtcTime (const util::Date &date, TimeZoneType zone);
                  UtcTime (const UtcTime &other);

   bool           is_valid () const;

   void           set (archi::Int64 new_val);
   archi::Int64   get () const;

   void           conv_to_date (util::Date &date, TimeZoneType zone) const;
   int            conv_from_date (const util::Date &date, TimeZoneType zone); // May fail ? (for example if local date is in a period skipped by DST/standard change ?)

   int            conv_to_c_time (ohm_archi_STD_NAME (time_t) &date_c) const;
   void           conv_from_c_time (ohm_archi_STD_NAME (time_t) date_c);

   archi::Int64   compute_diff_ticks (const UtcTime &other) const;
   double         compute_diff_s (const UtcTime &other) const;

   bool           operator < (const UtcTime &other) const;
   bool           operator == (const UtcTime &other) const;
   bool           operator != (const UtcTime &other) const;

   static void    get_current_time (UtcTime &t);

   static const archi::Int64
                  _ticks_per_sec;
   static const int
                  _reference_year;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   archi::Int64   _val; // 100 ns intervals from 1601.01.01, 0h 0min 0s. Negative value = invalid, not initialised



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:


}; // class UtcTime



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/UtcTime.hpp"



#endif   // ohm_sys_UtcTime_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
