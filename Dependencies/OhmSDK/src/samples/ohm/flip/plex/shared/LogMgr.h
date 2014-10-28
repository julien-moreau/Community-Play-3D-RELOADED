/*****************************************************************************

        LogMgr.h
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



#if ! defined (plex_LogMgr_HEADER_INCLUDED)
#define  plex_LogMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/shared/LogNamespace.h"

#include "ohm/txt/out/Print.h"

#define TRACEo(fm)   LogMgr::info (_ns_log_0, fm "").flush ()
#define TRACEo1(fm,x1)  LogMgr::info (_ns_log_0, fm "").arg (x1).flush ()
#define TRACEo2(fm,x1,x2)  LogMgr::info (_ns_log_0, fm "").arg (x1).arg (x2).flush ()
#define TRACEo3(fm,x1,x2,x3)  LogMgr::info (_ns_log_0, fm "").arg (x1).arg (x2).arg (x3).flush ()

#define TRACEI(fm)   LogMgr::error (_ns_log_0, fm "").flush ()
#define TRACEI1(fm,x1)  LogMgr::error (_ns_log_0, fm "").arg (x1).flush ()
#define TRACEI2(fm,x1,x2)  LogMgr::error (_ns_log_0, fm "").arg (x1).arg (x2).flush ()
#define TRACEI3(fm,x1,x2,x3)  LogMgr::error (_ns_log_0, fm "").arg (x1).arg (x2).arg (x3).flush ()

#define TRACEX(fm)   LogMgr::incident (_ns_log_0, fm "").flush ()
#define TRACEX1(fm,x1)  LogMgr::incident (_ns_log_0, fm "").arg (x1).flush ()
#define TRACEX2(fm,x1,x2)  LogMgr::incident (_ns_log_0, fm "").arg (x1).arg (x2).flush ()
#define TRACEX3(fm,x1,x2,x3)  LogMgr::incident (_ns_log_0, fm "").arg (x1).arg (x2).arg (x3).flush ()



namespace plex
{



class LogMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef const char * StrType;
   
   virtual        ~LogMgr () {}
   
   static LogMgr &
                  info (const char * namespace_0, const char * format_0);
   static LogMgr &
                  error (const char * namespace_0, const char * format_0);
   static LogMgr &
                  incident (const char * namespace_0, const char * format_0);
   LogMgr &       arg (const ohm::archi::Int64 & val);
   LogMgr &       arg (const ohm::archi::UInt64 & val);
   LogMgr &       arg (const long & val);
   LogMgr &       arg (const unsigned long & val);
   LogMgr &       arg (const int & val);
   LogMgr &       arg (const unsigned int & val);
   LogMgr &       arg (const void * val);
   LogMgr &       arg (const long double & val);
   LogMgr &       arg (const double & val);
   LogMgr &       arg (const StrType & val);
   LogMgr &       arg (const std::string & val);
   void           flush ();

   void           write (ohm::util::BinaryStreamOutputBase & bso) const;
   int            read (ohm::util::BinaryStreamInput & bsi);
   
   const LogNamespaceMap &
                  get_log_ns_map () const;
   
   static LogMgr &
                  use ();

   static const char *
                  _ns_incident_0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  LogMgr ();

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           print (LogLine::Type type, const char * namespace_0, const char * format_0);
   void           flush (const std::string & str, LogLine::Type type, time_t time, const char * namespace_0);
   
   LogLine::Type  _cur_type;
   time_t         _cur_time;
   const char *   _namespace_0;
   ohm::txt::out::Print
                  _printer;

   LogNamespaceMap
                  _log_ns_map;
   LogLineList    _incident_line_list;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  LogMgr (const LogMgr &other);
   LogMgr &       operator = (const LogMgr &other);
   bool           operator == (const LogMgr &other);
   bool           operator != (const LogMgr &other);

}; // class LogMgr



}  // namespace plex



//#include  "LogMgr.hpp"



#endif   // plex_LogMgr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

