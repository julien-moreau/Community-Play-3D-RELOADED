/*****************************************************************************

        MacTimeFnc.h
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



#if defined (ohm_sys_MacTimeFnc_CURRENT_HEADER)
   #error Recursive inclusion of ohm/sys/MacTimeFnc header.
#endif
#define  ohm_sys_MacTimeFnc_CURRENT_HEADER

#if ! defined (ohm_sys_MacTimeFnc_HEADER_INCLUDED)
#define  ohm_sys_MacTimeFnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace sys
{



class UtcTime;

class MacTimeFnc
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~MacTimeFnc ();
   
   static void    CFAbsoluteTime_to_UtcTime (UtcTime & utc_time, const ::CFAbsoluteTime & absolute_time);
   static void    UtcTime_to_CFAbsoluteTime (::CFAbsoluteTime & absolute_time, const UtcTime & utc_time);
   
   static void    UTCDateTime_to_UtcTime (UtcTime & utc_time, const ::UTCDateTime & utc_date_time);
   static void    UtcTime_to_UTCDateTime (::UTCDateTime & utc_date_time, const UtcTime & utc_time);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  MacTimeFnc ();
                  MacTimeFnc (const MacTimeFnc &other);
   MacTimeFnc &   operator = (const MacTimeFnc &other);
   bool           operator == (const MacTimeFnc &other);
   bool           operator != (const MacTimeFnc &other);

}; // class MacTimeFnc



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_MacTimeFnc_HEADER_INCLUDED

#undef ohm_sys_MacTimeFnc_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
