/*****************************************************************************

        LogNamespace.cpp
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

#include "plex/shared/LogNamespace.h"

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

LogNamespace::LogNamespace ()
:  _timestamp (::time (0))
,  _log_line_list ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

LogNamespace::~LogNamespace ()
{
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  LogNamespace::push (LogLine::Type type, time_t time, const std::string & str)
{
   _log_line_list.insert (_log_line_list.end (), LogLine (type, time, str));
   
   while (_log_line_list.size () > MAX_NBR_LINE)
   {
      _log_line_list.pop_front ();
   }
   
   _timestamp = ::time (0);
}



/*
==============================================================================
Name : make_html
==============================================================================
*/

void  LogNamespace::make_html (std::string & html) const
{
   LogLineList::const_iterator it = _log_line_list.begin ();
   const LogLineList::const_iterator it_end = _log_line_list.end ();
   
   for (; it != it_end ; ++it)
   {
      const LogLine & log_line = *it;
      
      log_line.make_html (html);
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  LogNamespace::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   bso.write (_namespace);
   bso.write (archi::UInt32 (_timestamp));
   bso.write (_log_line_list.begin (), _log_line_list.end ());
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   LogNamespace::read (ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (_namespace, "bsi:logns:namespace");
   bsi.read_uint32 (_timestamp, "bsi:logns:timestamp");
   bsi.read (_log_line_list, "bsi:logns:linelist");
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

