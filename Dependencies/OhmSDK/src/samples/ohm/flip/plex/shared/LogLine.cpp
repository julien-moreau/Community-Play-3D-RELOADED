/*****************************************************************************

        LogLine.cpp
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

#include "plex/shared/LogLine.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

LogLine::LogLine ()
:  _type (Type_INVALID)
,  _time (0)
,  _str ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

LogLine::LogLine (Type type, time_t time, const std::string & str)
:  _type (type)
,  _time (time)
,  _str (str)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

LogLine::~LogLine ()
{
}



/*
==============================================================================
Name : make_html
==============================================================================
*/

void  LogLine::make_html (std::string & html) const
{
   switch (_type)
   {
   case Type_INFO:
      html += "<div class=\"log_info\">";
      break;

   case Type_ERROR:
      html += "<div class=\"log_error\">";
      break;

   case Type_INCIDENT:
      html += "<div class=\"log_incident\">";
      break;
   }

   tm tm_time;
   ::gmtime_r (&_time, &tm_time);

   char date_0 [100];
   ::strftime (date_0, sizeof (date_0), "%a, %d %b %Y %H:%M:%S GMT", &tm_time);
   
   html += "<span class=\"log_date\">" + std::string (date_0) + "</span>";
   
   html += _str;
   
   html += "</div>\r\n";
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  LogLine::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   assert (_type >= Type_INFO);
   assert (_type <= Type_INCIDENT);
   
   bso.write (archi::UByte (_type));
   bso.write (archi::UInt32 (_time));
   bso.write (_str);
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   LogLine::read (ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_ubyte (_type, "bsi:logline:type");
   bsi.read_uint32 (_time, "bsi:logline:time");
   bsi.read (_str, "bsi:logline:str");
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

