/*****************************************************************************

        LogMgr.cpp
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

#include "plex/shared/LogMgr.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include "ohm/util/trace.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : info
==============================================================================
*/

LogMgr & LogMgr::info (const char * namespace_0, const char * format_0)
{
   LogMgr & log_mgr = use ();
   
   log_mgr.print (LogLine::Type_INFO, namespace_0, format_0);
   
   return log_mgr;
}



/*
==============================================================================
Name : error
==============================================================================
*/

LogMgr & LogMgr::error (const char * namespace_0, const char * format_0)
{
   LogMgr & log_mgr = use ();
   
   log_mgr.print (LogLine::Type_ERROR, namespace_0, format_0);
   
   return log_mgr;
}



/*
==============================================================================
Name : incident
==============================================================================
*/

LogMgr & LogMgr::incident (const char * namespace_0, const char * format_0)
{
   LogMgr & log_mgr = use ();
   
   log_mgr.print (LogLine::Type_INCIDENT, namespace_0, format_0);
   
   return log_mgr;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const ohm::archi::Int64 & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const ohm::archi::UInt64 & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const long & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const unsigned long & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const int & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const unsigned int & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const void * val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const long double & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const double & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const StrType & val)
{
   _printer.arg (val);
   
   return *this;
}



/*
==============================================================================
Name : arg
==============================================================================
*/

LogMgr & LogMgr::arg (const std::string & val)
{
   _printer.arg (val.c_str ());
   
   return *this;
}



/*
==============================================================================
Name : flush
==============================================================================
*/

void  LogMgr::flush ()
{
   const std::string & str = _printer;
   
   flush (str, _cur_type, _cur_time, _namespace_0);
   
   switch (_cur_type)
   {
   case LogLine::Type_INFO:
      ohm_util_TRACE_INFO2 ("%1%: %2%", _namespace_0, str.c_str ());
      break;

   case LogLine::Type_ERROR:
      ohm_util_TRACE_ERROR2 ("%1%: %2%", _namespace_0, str.c_str ());
      break;

   case LogLine::Type_INCIDENT:
      ohm_util_TRACE_PANIC2 ("%1%: %2%", _namespace_0, str.c_str ());
      flush (str, _cur_type, _cur_time, _ns_incident_0);
      break;
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  LogMgr::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   bso.write (_log_ns_map.begin (), _log_ns_map.end ());
   bso.write (_incident_line_list.begin (), _incident_line_list.end ());
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   LogMgr::read (ohm::util::BinaryStreamInput & bsi)
{
   _log_ns_map.clear ();
   bsi.read (_log_ns_map, "bsi:logmgr:log_ns_map");
   
   _incident_line_list.clear ();
   bsi.read (_incident_line_list, "bsi:logmgr:incidents");
   
   return 0;
}



/*
==============================================================================
Name : get_log_ns_map
==============================================================================
*/

const LogNamespaceMap & LogMgr::get_log_ns_map () const
{
   return _log_ns_map;
}



/*
==============================================================================
Name : use
==============================================================================
*/

LogMgr & LogMgr::use ()
{
   static LogMgr instance;
   
   return instance;
}



const char *   LogMgr::_ns_incident_0 = "Incidents";



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

LogMgr::LogMgr ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : print
==============================================================================
*/

void  LogMgr::print (LogLine::Type type, const char * namespace_0, const char * format_0)
{
   _cur_type = type;
   _cur_time = time (0);
   _namespace_0 = namespace_0;
   
   _printer.print (format_0);
}



/*
==============================================================================
Name : flush
==============================================================================
*/

void  LogMgr::flush (const std::string & str, LogLine::Type type, time_t time, const char * namespace_0)
{
   LogNamespace & log_ns = _log_ns_map [namespace_0];
   
   if (log_ns._namespace.empty ())
   {
      log_ns._namespace = namespace_0;
   }
   
   log_ns.push (type, time, str);
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

