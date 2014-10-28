/*****************************************************************************

        UtcTime_common.cpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18202

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

#include "ohm/lang/def.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/UtcTime.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <cassert>
#include <climits>

namespace std {}



namespace ohm
{
namespace sys
{



// Tick count at 1970.01.01, 00:00:00
static const archi::Int64  UtcTime_the_epoch =
   ohm_archi_MAKE_INT64 (0x019DB1DEL, 0xD53E8000UL);



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



UtcTime::UtcTime ()
:  _val (-1)
{
   // Nothing
}



UtcTime::UtcTime (ohm_archi_STD_NAME (time_t) date_c)
:  _val (-1)
{
   conv_from_c_time (date_c);
}



UtcTime::UtcTime (const util::Date &date, TimeZoneType zone)
:  _val (-1)
{
   assert (&date != 0);
   assert (zone >= 0);
   assert (zone < TimeZoneType_NBR_ELT);

   const int      ret_val = conv_from_date (date, zone);
   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR ("ctor(): Cannot construct from date.");
      throw std::runtime_error (
         "ohm::sys::UtcTime::UtcTime(): Cannot construct from date."
      );
   }
}



UtcTime::UtcTime (const UtcTime &other)
:  _val (other._val)
{
   assert (&other != 0);
}



bool  UtcTime::is_valid () const
{
   return (_val >= 0);
}



void  UtcTime::set (archi::Int64 new_val)
{
   assert (new_val >= 0);

   _val = new_val;
}



archi::Int64   UtcTime::get () const
{
   assert (is_valid ());

   return (_val);
}



// May fail if the time to be converted is out of the [1970 ; 2038] range.
int   UtcTime::conv_to_c_time (ohm_archi_STD_NAME (time_t) &date_c) const
{
   assert (is_valid ());
   assert (&date_c != 0);

   int            ret_val = Err_OK;

   if (_val < UtcTime_the_epoch)
   {
      ret_val = Err_DATE_OUT_OF_RANGE;
   }

   else
   {
      const archi::Int64   std_time =
         (_val - UtcTime_the_epoch) / _ticks_per_sec;
      if (std_time > 0xFFFFFFFFUL)
      {
         ret_val = Err_DATE_OUT_OF_RANGE;
      }

      else
      {
         using namespace std;
         date_c = static_cast <time_t> (std_time);
      }
   }

   return (ret_val);
}



void  UtcTime::conv_from_c_time (ohm_archi_STD_NAME (time_t) date_c)
{
   _val =
        UtcTime_the_epoch
      + static_cast <archi::Int64> (date_c) * _ticks_per_sec;
}



archi::Int64   UtcTime::compute_diff_ticks (const UtcTime &other) const
{
   assert (is_valid ());
   assert (&other != 0);
   assert (other.is_valid ());

   return (_val - other._val);
}



double   UtcTime::compute_diff_s (const UtcTime &other) const
{
   assert (is_valid ());
   assert (&other != 0);
   assert (other.is_valid ());

   const archi::Int64   ticks = compute_diff_ticks (other);
   const double   diff_s =
      static_cast <double> (ticks) / static_cast <double> (_ticks_per_sec);

   return (diff_s);
}



bool  UtcTime::operator < (const UtcTime &other) const
{
   assert (is_valid ());
   assert (&other != 0);
   assert (other.is_valid ());

   return (compute_diff_ticks (other) < 0);
}



bool  UtcTime::operator == (const UtcTime &other) const
{
   assert (is_valid ());
   assert (&other != 0);
   assert (other.is_valid ());

   return (compute_diff_ticks (other) == 0);
}



bool  UtcTime::operator != (const UtcTime &other) const
{
   assert (is_valid ());
   assert (&other != 0);
   assert (other.is_valid ());

   return (! ((*this) == other));
}



const archi::Int64   UtcTime::_ticks_per_sec = 10L * 1000L * 1000L;
const int   UtcTime::_reference_year = 1601;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Rough check
static const int  UtcTime_the_epoch_year = 1970;
static const int  UtcTime_diff_epoch_ref = UtcTime_the_epoch_year - UtcTime::_reference_year;
static const archi::Int64  UtcTime_nbr_ticks_per_day = UtcTime::_ticks_per_sec * 24 * 3600;
ohm_lang_CHECK_CST (EpochLB, UtcTime_the_epoch > UtcTime_diff_epoch_ref * 365 * UtcTime_nbr_ticks_per_day);
ohm_lang_CHECK_CST (EpochHB, UtcTime_the_epoch < UtcTime_diff_epoch_ref * 366 * UtcTime_nbr_ticks_per_day);



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
